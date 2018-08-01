#pragma once
#define  _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS


#include "MsgServer.h"
#include "../NoSqlDb/NoSqlDb.h"
//#include <iostream>
using namespace Logging;
using Show = StaticLogger<1>;
using namespace Utilities;
//constructor
MsgServer::MsgServer()
{
	


}
//split string with , as delimiter
std::vector<std::string> splitstring(std::string cssv)
{
	std::string s = cssv;
	std::string delimiter = ",";
	std::vector<std::string> names;
	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		names.push_back(token);
		s.erase(0, pos + delimiter.length());
	}

	return names;
}
//LAZY DOWNLOAD ALGORITHM
void MsgServer::LazySending(std::string paths, std::string catn)
{
	std::string temppath = "./ServerRepository/" + catn;
	std::vector<std::string> pathall = splitstring(paths);
	NoSqlDb<std::string>db;
	DBWrapper<std::string>dbw;
	db = dbw.restore(temppath+"/"+catn + ".xml");
	std::vector<std::string> fqfp;
	std::vector<std::string> keys;
	keys = db.keys();
	std::string newdlpaths;
	Element<std::string>ele;

		for (std::string k : keys)
		{
			ele = db.value(k).ret();
			std::string t = FileSystem::Path::getName(k);
			size_t p;
			std::string dl = "\n      ";
			std::string tok;
			while ((p = t.find(dl)) != std::string::npos)
			{
				tok = t.substr(0, p);
					t.erase(0, p + dl.length());
			}
			if (std::find(fqfp.begin(), fqfp.end(), FileSystem::Path::getName(tok)) == fqfp.end())
			fqfp.push_back(FileSystem::Path::getName(tok));
			for (std::string x : ele.childrel)
			{
				if (std::find(fqfp.begin(), fqfp.end(), FileSystem::Path::getName(x.substr(0,x.find(".htm")))) == fqfp.end())
					fqfp.push_back(FileSystem::Path::getName(x));
			}
		
	}
		for (std::string x : fqfp)
		{
			newdlpaths += x+".htm";
			newdlpaths += ",";
		}
		sendingFile(newdlpaths, catn);
}
//sending multiple files
void MsgServer::sendingFile(std::string paths,std::string catn)
{
	std::string temppath = "./ServerRepository/" + catn + "/";
	std::vector<std::string> pathall = splitstring(paths);
	try {

		while (!si.connect("localhost", port))
		{
			Show::write("\n server waiting to connect");
			::Sleep(100);
		}

		HttpMessage msg;
		
		for (size_t i = 0; i < pathall.size(); i++)
		{
			std::cout << "Sending File" << pathall[i] << "\n";
			sendFile(temppath+pathall[i], si);
		}
		HttpMessage qMsg = makeMessage(1, "quit", toaddr);
		sendMessage(qMsg, si);
		
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
		
	}
}
//sending file names
void MsgServer::getnsendfilesname(std::string path)
{
	std::string result="ReqFiles/";
	std::vector<std::string> filenames;
	filenames = FileSystem::Directory::getFiles(path, "*.htm");
	for (std::string x : filenames)
	{
		result += x;
		result += ",";	}
	if (filenames.empty() == false)
	{
		try
		{
			while (!si.connect("localhost", port))
			{
				Show::write("\n server waiting to connect");
				::Sleep(100);}
			std::cout << "\nSending FileNames\n";
			HttpMessage msg=makeMessage(1,result,toaddr);
	sendMessage(msg, si);
			std::cout << "\nServer sending QuitMessage\n";
			HttpMessage qMsg = makeMessage(1, "quit", toaddr);
			sendMessage(qMsg, si);}
		catch (std::exception& exc)
		{
			Show::write("\n  Exeception caught: ");
			std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
			Show::write(exMsg);}
	}
	else
	{
		try
		{
			while (!si.connect("localhost", port))
			{
				Show::write("\n server waiting to connect");
				::Sleep(100);
			}
			std::cout << "\nNo Files Found\n";
			std::cout << "\nServer sending QuitMessage\n";
			HttpMessage qMsg = makeMessage(1, "quit", toaddr);
			sendMessage(qMsg, si);
		}
		catch (std::exception& exc)
		{
			Show::write("\n  Exeception caught: ");
			std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
			Show::write(exMsg);
		}
	}
}
//gets all Independent file names
std::vector<std::string> GetIFiles(std::string path,std::string cat)
{
	
	NoSqlDb<std::string> db;
	DBWrapper<std::string> dbw;
	std::vector<std::string> Ifiles;
   std::string rp = path + cat + "/" + cat + ".xml";
	db = dbw.restore(path+"/"+cat+".xml");
	std::vector<std::string> keys;
	bool chck = true;
	for (std::string k : db.keys())
	{
		Element<std::string> ele;
		size_t pos;
		pos = k.find("\n");
		std::string m = k.substr(0, pos);
		for (std::string tk : db.keys())
		{
			ele = db.value(tk).ret();
			if (std::find(ele.childrel.begin(), ele.childrel.end(), m) != ele.childrel.end())
				chck = false;
		}
		if (chck == true)
			Ifiles.push_back(m);
		else
			chck = true;
	}
	for (size_t i = 0; i < Ifiles.size(); i++)
	{
		Ifiles[i] += ".htm";
	}
	return Ifiles;
}
//wrapper fn for getting all files with no parent(independent files)
void MsgServer::getnsendIfilename(std::string path,std::string cat)
{
	std::string result="ReqFiles/";
	std::vector<std::string> filenames;
	filenames = GetIFiles(path, cat);
	for (std::string x : filenames)
	{
		result += FileSystem::Path::getName(x);
		result += ",";
	}
	if (filenames.empty() == false)
	{try
		{
			while (!si.connect("localhost", port))
			{
				Show::write("\n server waiting to connect");
				::Sleep(100);
			}
			std::cout << "\nSending FileNames\n";
			HttpMessage msg=makeMessage(1,result, toaddr);
			sendMessage(msg, si);	
			std::cout << "\nServer sending QuitMessage\n";
			HttpMessage qMsg = makeMessage(1, "quit", toaddr);
			sendMessage(qMsg, si);
		}
		catch (std::exception& exc)
		{
			Show::write("\n  Exeception caught: ");
			std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
			Show::write(exMsg);
		}}
	else
	{
		try
		{while (!si.connect("localhost", port))
			{
				Show::write("\n server waiting to connect");
				::Sleep(100);
			}
			std::cout << "\nServer sending QuitMessage\n";
			HttpMessage qMsg = makeMessage(1, "quit", toaddr);
			sendMessage(qMsg, si);
		}
		catch (std::exception& exc)
		{
			Show::write("\n  Exeception caught: ");
			std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
			Show::write(exMsg);
		}
	}
}
//make message function to create http messages
HttpMessage MsgServer::makeMessage(size_t n, const std::string& body, const EndPoint& ep)
{
	HttpMessage msg;
	HttpMessage::Attribute attrib;
	EndPoint myEndPoint = "localhost:8080";  // ToDo: make this a member of the sender
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

void MsgServer::sendMessage(HttpMessage& msg, Socket& socket)
{
	std::string msgString = msg.toString();
	socket.send(msgString.size(), (Socket::byte*)msgString.c_str());
}
bool MsgServer::sendFile(const std::string& filename, Socket& socket)
{
	// assumes that socket is connected

	std::string fqname =  filename;
	FileSystem::FileInfo fi(fqname);
	size_t fileSize = fi.size();
	std::string sizeString = Converter<size_t>::toString(fileSize);
	FileSystem::File file(fqname);
	file.open(FileSystem::File::in, FileSystem::File::binary);
	if (!file.isGood())
		return false;

	HttpMessage msg = makeMessage(1, "", toaddr);
	msg.addAttribute(HttpMessage::Attribute("file", filename));
	msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));
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

/////////////////////////////////////////////////////////////////////
// ClientHandler class
/////////////////////////////////////////////////////////////////////
// - instances of this class are passed by reference to a SocketListener
// - when the listener returns from Accept with a socket it creates an
//   instance of this class to manage communication with the client.
// - You need to be careful using data members of this class
//   because each client handler thread gets a reference to this 
//   instance so you may get unwanted sharing.
// - I may change the SocketListener semantics (this summer) to pass
//   instances of this class by value.
// - that would mean that all ClientHandlers would need either copy or
//   move semantics.

class ClientHandler
{
public:
	ClientHandler(BlockingQueue<HttpMessage>& msgQ) : msgQ_(msgQ) {}
	void operator()(Socket socket);
private:
	bool connectionClosed_;
	HttpMessage readMessage(Socket& socket);
	bool readFile(const std::string& filename, size_t fileSize, Socket& socket,std::string);
	BlockingQueue<HttpMessage>& msgQ_;
	bool publish(std::string);
	void deletefiles(std::string path, std::string cat);
};
//delete files
void ClientHandler::deletefiles(std::string path, std::string cat)
{
	
		std::vector<std::string> filenames;
		filenames = splitstring(path);
		std::string temppath = "./ServerRepository/" + cat + "/";
		for (std::string x : filenames)
		{
			std::string newtemp = temppath + FileSystem::Path::getName(x);
			std::string newtemp2 = newtemp;
			newtemp2 = newtemp2.substr(0, newtemp2.find(".htm"));
			if (std::remove(newtemp.c_str()) != 0)
				std::cout << "\nError Deleting File:" << newtemp << "\n";
			else
				std::cout << "\nDeleted File:" << newtemp << "\n";
			if (std::remove(newtemp2.c_str()) != 0)
				std::cout << "\nError Deleting File:" << newtemp2 << "\n";
			else
				std::cout << "\nDeleted File:" << newtemp2 << "\n";
		}
		std::cout << "\n-------Publishing again to maintain integrity-----\n";
		publish(cat);
	
}
//publish function
bool ClientHandler::publish(std::string catname)
{
	try {
		CodeAnalysis::CodeAnalysisExecutive exec;
		HttpMessage msg;
		char* abc[5];
		abc[0] = "Executive.exe";
		std::string temp = "./ServerRepository/" + catname+"/";
		char * writable = new char[temp.size() + 1];
		std::copy(temp.begin(), temp.end(), writable);
		writable[temp.size()] = '\0';
		abc[1] = writable;
		abc[2] = "*.h";
		abc[3] = "*.cpp";
		std::string temp2 = temp+catname + ".xml";
		char * writable2 = new char[temp2.size() + 1];
		std::copy(temp2.begin(), temp2.end(), writable2);
		writable2[temp2.size()] = '\0';
		abc[4] = writable2;
		exec.execute(5, abc);
		return true;
	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
		return false;
	}
}

//setup iis function
void MsgServer::Setupiis(std::string path)
{
	std::string root = path;

	FileSystem::Directory::create(root + "/CategoryOne");
	FileSystem::Directory::create(root + "/CategoryTwo");
	FileSystem::Directory::create(root + "/CategoryThree");
	FileSystem::Directory::create(root + "/DefaultCategory");
	FileSystem::Directory::create(root + "/Common");
	std::string sroot = "./ServerRepository";
	std::vector<std::string> c1fn = FileSystem::Directory::getFiles(sroot+"/"+"CategoryOne");
	std::vector<std::string> c2fn = FileSystem::Directory::getFiles(sroot + "/" + "CategoryTwo");
	std::vector<std::string> c3fn = FileSystem::Directory::getFiles(sroot + "/" + "CategoryThree");
	std::vector<std::string> cdfn = FileSystem::Directory::getFiles(sroot + "/" + "DefaultCategory");
	std::vector<std::string> ccfn = FileSystem::Directory::getFiles(sroot + "/" + "Common");
	for (std::string x : c1fn)
	{
		std::string filename = sroot + "/CategoryOne/" + x;
		std::string dest = root + "/CategoryOne/"+x;
		bool check=FileSystem::File::copy(filename, dest,true);
		
	}
	for (std::string x : c2fn)
	{
		std::string filename = sroot + "/CategoryTwo/" + x;
		std::string dest = root + "/CategoryTwo/"+x;
		FileSystem::File::copy(filename, dest,true);
	}
	for (std::string x : c3fn)
	{
		std::string filename = sroot + "/CategoryThree/" + x;
		std::string dest = root + "/CategoryThree/"+x;
		FileSystem::File::copy(filename, dest,true);
	}
	for (std::string x : cdfn)
	{
		std::string filename = sroot + "/DefaultCategory/" + x;
		std::string dest = root + "/DefaultCategory/"+x;
		FileSystem::File::copy(filename, dest,true);
	}
	for (std::string x : ccfn)
	{
		std::string filename = sroot + "/Common/" + x;
		std::string dest = root + "/Common/"+x;
		FileSystem::File::copy(filename, dest,true);

	}
	createnewindexpage(root);
	}
//create index page
void MsgServer::createnewindexpage(std::string path)
{
	std::ofstream myfile;
	std::string x = path+"/"+"index.htm";
	myfile.open(x);
	myfile << "<!DOCTYPE html><html>";
	myfile << "<link rel=""stylesheet ""type=""text/css ""href=\"./Common/Stylesheet.css\"""/>";
	myfile << "<title> TABLE OF CONTENTS</title>";
	myfile << "<header><h1>TABLE OF CONTENTS</h1></header>";
	myfile << "<body>";
	std::vector<std::string> files;
	files.push_back("./CategoryOne/CategoryOne.htm");
	files.push_back("./CategoryTwo/CategoryTwo.htm");
	files.push_back("./DefaultCategory/DefaultCategory.htm");
	files.push_back("./CategoryThree/CategoryThree.htm");
	for (std::string file : files)
	{
		myfile << "<a href = \"" << file << "\"" << ">" << file;
		myfile << "</a>";
		myfile << "<br>";
	}
	myfile << "</body>";
	myfile << "</html>";
	myfile.close();
	std::cout << "\n---------Created Table of Contents Page----------\n";
	std::cout << "Page name is : MainPage.htm and is saved in root repo folder\n";
	

}
//read Message from client
HttpMessage ClientHandler::readMessage(Socket& socket)
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
			break;}
	if (msg.attributes().size() == 0)
	{
		connectionClosed_ = true;
		return msg;}
	if (msg.attributes()[0].first == "POST")
	{	std::string filename = msg.findValue("file");
		if (filename != "")
		{	size_t contentSize;
			std::string sizeString = msg.findValue("content-length");
			if (sizeString != "")
				contentSize = Converter<size_t>::toValue(sizeString);
			else
				return msg;
			std::string cat = msg.findValue("Category");
			readFile(filename, contentSize, socket,cat);}
		if (filename != "")
		{msg.removeAttribute("content-length");
			std::string bodyString = "<file>" + filename + "</file>";
			std::string sizeString = Converter<size_t>::toString(bodyString.size());
			msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));
			msg.addBody(bodyString);}
		else
		{
		if (msg.findValue("Publish").empty() == false)
			{
				std::string cat = msg.findValue("Publish");
				publish(cat);}
			else if (msg.findValue("Delete").empty() == false)
				deletefiles(msg.findValue("Delete"), msg.findValue("catname"));
			size_t numBytes = 0;
			size_t pos = msg.findAttribute("content-length");
			if (pos < msg.attributes().size())
			{	numBytes = Converter<size_t>::toValue(msg.attributes()[pos].second);
				Socket::byte* buffer = new Socket::byte[numBytes + 1];
				socket.recv(numBytes, buffer);
				buffer[numBytes] = '\0';
				std::string msgBody(buffer);
				msg.addBody(msgBody);
				delete[] buffer;}}}return msg;}
//----< read a binary file from socket and save >--------------------
/*
* This function expects the sender to have already send a file message,
* and when this function is running, continuosly send bytes until
* fileSize bytes have been sent.
*/
bool ClientHandler::readFile(const std::string& filename, size_t fileSize, Socket& socket,std::string category)
{
	std::string fqname = "./ServerRepository/"+category+"/" + FileSystem::Path::getName(filename);
	FileSystem::File file(fqname);
	file.open(FileSystem::File::out, FileSystem::File::binary);
	if (!file.isGood())
	{
		/*
		* This error handling is incomplete.  The client will continue
		* to send bytes, but if the file can't be opened, then the server
		* doesn't gracefully collect and dump them as it should.  That's
		* an exercise left for students.
		*/
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
	std::cout << "\n Recieved File:" << file.name();
	return true;
}
//----< receiver functionality is defined by this function >---------

void ClientHandler::operator()(Socket socket)
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

		if (connectionClosed_ || msg.bodyString() == "quit")
		{
			Show::write("\n\n  clienthandler thread is terminating");
			break;
		}
		msgQ_.enQ(msg);
	}
}

//----< test stub >--------------------------------------------------

int main()
{	::SetConsoleTitle(L"HttpMessage Server - Runs Forever");
	Show::attach(&std::cout);
	Show::start();
	Show::title("\n  HttpMessage Server started");
	BlockingQueue<HttpMessage> msgQ;
	MsgServer msgs;
	try
	{
		SocketSystem ss;
		SocketListener sl(8080, Socket::IP6);
		ClientHandler cp(msgQ);
		sl.start(cp);
		while (true)
		{
			HttpMessage msg = msgQ.deQ();
			msgs.toaddr = msg.findValue("fromAddr");
			std::string temp = msgs.toaddr.substr(msgs.toaddr.find(":")+1,msgs.toaddr.size());
			int tempst = Converter<int>::toValue(temp);
			msgs.port = tempst;
			Show::write("\n\n  server recvd message contents:\n" + msg.bodyString());
			if (msg.findValue("download").empty()==false)
			{	msgs.sendingFile(msg.findValue("download"), msg.findValue("catname"));		}
			else if (msg.findValue("Lazy").empty() == false)
			{	msgs.LazySending(msg.findValue("Lazy"), msg.findValue("catname"));
			}
			else if (msg.findValue("GetFiles").empty() == false)
			{
				std::string cat = msg.findValue("GetFiles");
				std::string temppath = "./ServerRepository/" + cat + "/";
				 msgs.getnsendfilesname(temppath);
			}
			else if (msg.findValue("GetIFiles").empty() == false)
			{
				std::string cat = msg.findValue("GetIFiles");
				std::string temppath = "./ServerRepository/" + cat + "/";
				msgs.getnsendIfilename(temppath,cat);
			}
			else if (msg.findValue("IIS").empty() == false)
			{
				std::string p = msg.findValue("IIS");
				msgs.Setupiis(p);
			}}	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exeception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}}