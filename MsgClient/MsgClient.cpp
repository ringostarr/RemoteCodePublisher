#pragma once
#include "../HttpMessage/HttpMessage.h"
#include "Sockets.h"
#include "FileSystem.h"
#include "../Logger/Logger.h"
#include "MsgClient.h"
#include "../Utilities/Utilities.h"
#include <string>
#include <iostream>
#include <thread>
using namespace Logging;
using Show = StaticLogger<1>;
using namespace Utilities;
using Utils = StringHelper;
static bool ft = false;
static std::string FileNames;
//read message from server
HttpMessage serverhandler::readMessage(Socket& socket)
{
	connectionClosed_ = false;
	HttpMessage msg;
	while (true)
	{
		std::string attribString = socket.recvString('\n');
		if (attribString.size() > 1)
		{	HttpMessage::Attribute attrib = HttpMessage::parseAttribute(attribString);
			msg.addAttribute(attrib);}
		else
		break;		}
	if (msg.attributes().size() == 0)
	{
		connectionClosed_ = true;
		return msg;}
	if (msg.attributes()[0].first == "POST")
	{
		std::string filename = msg.findValue("file");
		if (filename != "")
		{
			size_t contentSize;
			std::string sizeString = msg.findValue("content-length");
			if (sizeString != "")
				contentSize = Converter<size_t>::toValue(sizeString);
			else
				return msg;
			
			readFile(filename, contentSize, socket);
		}
		if (filename != "")
		{
			msg.removeAttribute("content-length");
			std::string bodyString = "<file>" + filename + "</file>";
			std::string sizeString = Converter<size_t>::toString(bodyString.size());
			msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));
			msg.addBody(bodyString);}
		else
		{
			size_t numBytes = 0;
			size_t pos = msg.findAttribute("content-length");
			if (pos < msg.attributes().size())
			{
				numBytes = Converter<size_t>::toValue(msg.attributes()[pos].second);
				Socket::byte* buffer = new Socket::byte[numBytes + 1];
				socket.recv(numBytes, buffer);
				buffer[numBytes] = '\0';
				std::string msgBody(buffer);
				msg.addBody(msgBody);
				delete[] buffer;
			}}}	return msg;}
//send message for independent filenames
std::string MsgClient::ReqIFL(std::string cat)
{


	try
	{
		HttpMessage msg = makeMessage(1, "GetIFiles", "localhost:8080");
		msg.addAttribute(HttpMessage::attribute("GetIFiles", cat));
		sendMessage(msg, si);

	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
	try
	{
		BlockingQueue<HttpMessage> msgQ;
		SocketListener sl(Converter<int>::toValue(this->port), Socket::IP6);
		serverhandler sh(msgQ);
		sh.RepoPath = this->repoPath;
		sl.start(sh);
		while (true)
		{
			if (ft)
				break;
		}
		ft = false;
		sl.close();
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}

	return FileNames;
}
// send message for filenames 
std::string MsgClient::ReqFiles(std::string cat)
{
	

	try
	{
		HttpMessage msg = makeMessage(1, "GetFiles", "localhost:8080");
		msg.addAttribute(HttpMessage::attribute("GetFiles", cat));
		sendMessage(msg, si);
		
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
	try
	{
		BlockingQueue<HttpMessage> msgQ;
		SocketListener sl(Converter<int>::toValue(this->port), Socket::IP6);
		serverhandler sh(msgQ);
		sh.RepoPath = this->repoPath;
		sl.start(sh);
		while (true)
		{
			/*HttpMessage msg = msgQ.deQ();*/
			if (ft)
				break;
		}
		ft = false;
		sl.close();
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}

	return FileNames;
}
//----< read a binary file from socket and save >--------------------
/*
* This function expects the sender to have already send a file message,
* and when this function is running, continuosly send bytes until
* fileSize bytes have been sent.
*/
bool serverhandler::readFile(const std::string& filename, size_t fileSize, Socket& socket)
{
	std::string catn;
	std::string temp = filename;
	if (temp.find("CategoryOne") != std::string::npos)
		catn = "CategoryOne";
	else 	if (temp.find("CategoryTwo") != std::string::npos)
		catn = "CategoryTwo";
	else 	if (temp.find("CategoryThree") != std::string::npos)
		catn = "CategoryThree";
	else
		catn = "DefaultCategory";
	std::string x=FileSystem::Path::getName(filename);
	std::cout << "\n Receiving File:" << x;
	std::string fqname = FileSystem::Directory::getCurrentDirectory()+"/"+this->RepoPath+catn+"/" + x;
	FileSystem::File file(fqname);
	file.open(FileSystem::File::out, FileSystem::File::binary);
	if (!file.isGood())
	{
		Show::write("\n\n  can't open file " + fqname);
		return false;
	}

	const size_t BlockSize = 2048;
	Socket::byte buffer[BlockSize];

	size_t bytesToRead;
	while (true)
	{
		if (fileSize > BlockSize)
			bytesToRead = BlockSize;
		else
			bytesToRead = fileSize;
		socket.recv(bytesToRead, buffer);
		FileSystem::Block blk;
		for (size_t i = 0; i < bytesToRead; ++i)
			blk.push_back(buffer[i]);

		file.putBlock(blk);
		if (fileSize < BlockSize)
			break;
		fileSize -= BlockSize;
	}
	file.close();
	return true;
}
//----< receiver functionality is defined by this function >---------

void serverhandler::operator()(Socket socket)
{
	/*
	* There is a potential race condition due to the use of connectionClosed_.
	* If two clients are sending files at the same time they may make changes
	* to this member in ways that are incompatible with one another.  This
	* race is relatively benign in that it simply causes the readMessage to
	* be called one extra time.
	*
	* The race is easy to fix by changing the socket listener to pass in a
	* copy of the clienthandler to the clienthandling thread it created.
	* I've briefly tested this and it seems to work.  However, I did not want
	* to change the socket classes this late in your project cycle so I didn't
	* attempt to fix this.
	*/
	while (true)
	{
		HttpMessage msg = readMessage(socket);
		if (msg.bodyString().substr(0, 8) == "ReqFiles")
		{
			msg.bodyString().erase(0, 9);
			std::string nres = msg.bodyString();
			nres.erase(0, 9);
			FileNames = nres;
		}
		else if (msg.bodyString().find("downloadf") != std::string::npos)
		{
			//HttpMessage asd=msgQ_.deQ();
		}
		if (msg.bodyString() == "quit")
		{
			Show::write("\n\n  serverHandler thread is terminating");
			ft = true;
			break;
		}
		msgQ_.enQ(msg);
	}
}
//default constructor
MsgClient::MsgClient()
{

	try {
		while (!si.connect("localhost", 8080))
		{
			Show::write("\n client waiting to connect");
			::Sleep(100);
		}
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
	
}
//paramtereized constructor
MsgClient::MsgClient(std::string path,std::string pn)
{
	repoPath = path;
	port = pn;

	try {
		while (!si.connect("localhost", 8080))
		{
			Show::write("\n client waiting to connect");
			::Sleep(100);
		}
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}

}
//send setup iis message
void MsgClient::SetupIIS(std::string path)
{
	HttpMessage msg;
	msg = makeMessage(1, "IIS", "localhost:8080");
	msg.addAttribute(HttpMessage::Attribute("IIS", path));
	sendMessage(msg, si);

}
//create http style messages
HttpMessage MsgClient::makeMessage(size_t n, const std::string& body, const EndPoint& ep)
{
	HttpMessage msg;
	HttpMessage::Attribute attrib;
	EndPoint myEndPoint = this->port;  // ToDo: make this a member of the sender
											 // given to its constructor.
	switch (n)
	{
	case 1:
		msg.clear();
		msg.addAttribute(HttpMessage::attribute("POST", "Message"));
		msg.addAttribute(HttpMessage::Attribute("mode", "oneway"));
		msg.addAttribute(HttpMessage::parseAttribute("toAddr:" + ep));
		msg.addAttribute(HttpMessage::parseAttribute("fromAddr:" + myEndPoint));

		msg.addBody(body);
		if (body.size() > 0)
		{
			attrib = HttpMessage::attribute("content-length", Converter<size_t>::toString(body.size()));
			msg.addAttribute(attrib);
		}
		break;
	default:
		msg.clear();
		msg.addAttribute(HttpMessage::attribute("Error", "unknown message type"));
	}
	return msg;
}
//----< send message using socket >----------------------------------

void MsgClient::sendMessage(HttpMessage& msg, Socket& socket)
{
	std::string msgString = msg.toString();
	socket.send(msgString.size(), (Socket::byte*)msgString.c_str());
}
//send publish message
void MsgClient::SendPublishMessage(std::string category)
{
	HttpMessage msg = makeMessage(1, "", "localhost::8080");
	msg.addAttribute(HttpMessage::Attribute("Publish", category));
	sendMessage(msg, si);
}
//----< send file using socket >-------------------------------------
/*
* - Sends a message to tell receiver a file is coming.
* - Then sends a stream of bytes until the entire file
*   has been sent.
* - Sends in binary mode which works for either text or binary.
*/

bool MsgClient::sendFile(const std::string& filename, Socket& socket,std::string category)
{
	// assumes that socket is connected

	std::string fqname = filename;
	FileSystem::FileInfo fi(fqname);
	size_t fileSize = fi.size();
	std::string sizeString = Converter<size_t>::toString(fileSize);
	FileSystem::File file(fqname);
	file.open(FileSystem::File::in, FileSystem::File::binary);
	if (!file.isGood())
		return false;

	HttpMessage msg = makeMessage(1, "", "localhost::8080");
	msg.addAttribute(HttpMessage::Attribute("file", filename));
	msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));
	msg.addAttribute(HttpMessage::Attribute("Category", category));
	sendMessage(msg, socket);
	const size_t BlockSize = 2048;
	Socket::byte buffer[BlockSize];
	while (true)
	{
		FileSystem::Block blk = file.getBlock(BlockSize);
		if (blk.size() == 0)
			break;
		for (size_t i = 0; i < blk.size(); ++i)
			buffer[i] = blk[i];
		socket.send(blk.size(), buffer);
		if (!file.isGood())
			break;
	}
	file.close();
	return true;
}
//send lazy download message to server
void MsgClient::LazyDL(std::string catname, std::string filnames)
{
	HttpMessage msg;
	msg = makeMessage(1, "Lazy", "localhost:8080");
	msg.addAttribute(HttpMessage::Attribute("Lazy", filnames));
	msg.addAttribute(HttpMessage::Attribute("catname", catname));
	sendMessage(msg, si);
	try
	{
		BlockingQueue<HttpMessage> msgQ;
		SocketListener sl(Converter<int>::toValue(this->port), Socket::IP6);
		serverhandler sh(msgQ);
		sh.RepoPath = this->repoPath;
		sl.start(sh);
		while (true)
		{
			if (ft)
				break;
		}
		ft = false;
		sl.close();
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}
//send delete message
void MsgClient::SendDeletemsg(std::string catname, std::string filnames)
{
	HttpMessage msg;
	msg = makeMessage(1, "Delete", "localhost:8080");


	msg.addAttribute(HttpMessage::Attribute("Delete", filnames));
	msg.addAttribute(HttpMessage::Attribute("catname", catname));
	sendMessage(msg, si);
	
}
//send download files message
void MsgClient::downloadFiles(std::string catname, std::string filnames)
{
	HttpMessage msg;
	msg=makeMessage(1, "download", "localhost:8080");
	
	
	msg.addAttribute(HttpMessage::Attribute("download", filnames));
	msg.addAttribute(HttpMessage::Attribute("catname", catname));
	sendMessage(msg, si);
	try
	{
		BlockingQueue<HttpMessage> msgQ;
		SocketListener sl(Converter<int>::toValue(this->port), Socket::IP6);
		serverhandler sh(msgQ);
		sh.RepoPath = this->repoPath;
		sl.start(sh);
		while (true)
		{
			if (ft)
				break;
		}
		ft = false;
		sl.close();
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}
//send upload message
void MsgClient::upload(std::string catname,std::vector<std::string> path)
{
	try
	{
		
		for (size_t i = 0; i < path.size(); i++)
		{
			std::string temp = path[i].substr(0,5);
			if (temp == "(Dir)")
			{
				std::vector<std::string> files = FileSystem::Directory::getFiles(path[i].substr(6, path[i].size()), "*.cpp");
				for (size_t j=0;j<files.size();j++)
				{
					files[j].insert(0,path[i].substr(6, path[i].size())+"/");
				}
				for (size_t j = 0; j < files.size(); j++)
				{
					sendFile(FileSystem::Path::getFullFileSpec((files[i])), si,catname);
				}
				std::vector<std::string> hfiles = FileSystem::Directory::getFiles(path[i].substr(6, path[i].size()), "*.h");
				for (size_t j = 0; j<hfiles.size(); j++)
				{
					hfiles[j].insert(0,path[i].substr(6, path[i].size())+ "/" );
				}
				for (size_t j = 0; j < hfiles.size(); j++)
				{
					sendFile(FileSystem::Path::getFullFileSpec((hfiles[i])), si,catname);
				}
			}
			else
			{
				std::string impfn = path[i].substr(path[i].size() - 4, path[i].size());
				std::string hfn = path[i].substr(path[i].size() - 3, path[i].size());
				if(path[i].substr(path[i].size()-4,path[i].size())==".cpp" || path[i].substr(path[i].size() - 2, path[i].size()) == ".h")
				sendFile(FileSystem::Path::getFullFileSpec(path[i]), si,catname);
			}
			}
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}
//----< this defines the behavior of the client >--------------------
//----< entry point - runs console client each on its own thread >------

int main()
{
	::SetConsoleTitle(L"Client2 Running on Different Thread");
	std::cout << "=====================================================================";
	std::cout << "PROJECT4 -DEMO , This is Client2,my path is ./Client2Repo============";

	std::cout << "=====================================================================";
	std::cout << " Requirement6,7-Every Connection from Client to Server has been done using TCP sockets,Messages sent are always in the form of Http Messages\n";
	std::cout << "Requirement 5 : Sending Upload File Message for files given below , Please check Server console for more info\n";
	MsgClient c3("./Client2Repo/","8082");
	std::string catname = "CategoryTwo";
	std::vector<std::string> fn;
	fn.push_back(FileSystem::Directory::getCurrentDirectory()+"/TA-TEST-PROJECT-2/files/Parent.h");
	fn.push_back(FileSystem::Directory::getCurrentDirectory() + "/TA-TEST-PROJECT-2/files/Invalid.h");
	fn.push_back(FileSystem::Directory::getCurrentDirectory() + "/TA-TEST-PROJECT-2/files/Child/Child2.h");
	fn.push_back(FileSystem::Directory::getCurrentDirectory() + "/TA-TEST-PROJECT-2/files/Child/Child.cpp");
	fn.push_back(FileSystem::Directory::getCurrentDirectory() + "/TA-TEST-PROJECT-2/files/Child/Child.h");
	fn.push_back(FileSystem::Directory::getCurrentDirectory() + "/TA-TEST-PROJECT-2/files/Test.cpp");
	fn.push_back(FileSystem::Directory::getCurrentDirectory() + "/TA-TEST-PROJECT-2/files/Parent.cpp");
	fn.push_back(FileSystem::Directory::getCurrentDirectory() + "/TA-TEST-PROJECT-2/files/GrandParent.h");
	std::cout << "\n uploading files to category two:\n";
	for (std::string x : fn)
	{
		std::cout <<x;
		std::cout << "\n";
	}
	std::string callbackmessage;
	std::thread t1([&]() {c3.upload(catname, fn);
	std::cout << "\n\n-Requirement 3,4-Sending Publishing Message to server for Category Two, Please check server console for more info\n";
	c3.SendPublishMessage(catname); 
	std::cout << "\n\n-Requirement 8,10 - Sending Lazy Download message ,Please check server console and see if server sends the files back\n";
	std::cout << "\nLazy Downloading Parent.h.htm\n";
	std::cout << "\n Should receive all files except Invalid.h,check client2repo folder\n";
	std::cout << "requirement 1,2 has been satisified, check code for furthur details\n";
	std::cout << "Requirement 11- Please enter IIS path in textbox and click the IIS Button";
	std::cout << "\n For Other functionality mentioned in the purpose section , USE THE GUI\n";
	c3.LazyDL(catname, "Parent.h.htm,");});
	t1.join();
	std::cin.get();
	
	
}
