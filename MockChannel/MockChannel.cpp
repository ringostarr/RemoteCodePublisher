
/////////////////////////////////////////////////////////////////////
// MockChannel.cpp - Connection between gui and console client     //
//  Akshay,
//  708,S beech Street #1 ,Syracuse .
//  Akshay@syr.edu
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   CSE687 Pr1, Spring 2017
//	Reference: Jim Fawcett, CSE687 - Object Oriented Design, Spring 2015  //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
Creates a MockChannel which communicates between GUI and MsgClient

Public Interface:
=================
class Sendr() - Sendr class - accept message for consumption by mockchannel
class recvr() - Recvr class - same as sender class
class Mockchannel(() - consumes the message sent
-processmessage() - processes msg received and calls appropriate function from msg client
-start() - start thread to read from queue
-stop() - stop above thread
splitstring(std::string) - splits string "," is default delimiter(cannot be changed)
Build Process:
==============
Required files
- MockChannel.h , MockChannel.cpp,MsgClient.h,Msgclient.cpp,Msgserver.cpp,MsgServer.h
Build commands (either one)
- devenv oodp26.sln
- cl /EHsc /TEST_DEP Dependency.cpp /link setargv.obj

Maintenance History:
====================
ver2.0 : CHanged for compatibilty with project 4
ver1.0 : Initial commit

*/
#define IN_DLL
#include "MockChannel.h"
#include "../MsgClient/MsgClient.h"
#include "../Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"
using BQueue = BlockingQueue < Message >;

/////////////////////////////////////////////////////////////////////////////
// Sendr class
// - accepts messages from client for consumption by MockChannel
//

class Sendr : public ISendr
{
public:
	void postMessage(const Message& msg);
	BQueue& queue();
private:
	BQueue sendQ_;
};

void Sendr::postMessage(const Message& msg)
{
	sendQ_.enQ(msg);
}

BQueue& Sendr::queue() { return sendQ_; }

/////////////////////////////////////////////////////////////////////////////
// Recvr class
// - accepts messages from MockChanel for consumption by client
//
class Recvr : public IRecvr
{
public:
	Message getMessage();
	BQueue& queue();
private:
	BQueue recvQ_;
};

Message Recvr::getMessage()
{
	return recvQ_.deQ();
}

BQueue& Recvr::queue()
{
	return recvQ_;
}
/////////////////////////////////////////////////////////////////////////////
// MockChannel class
// - reads messages from Sendr and writes messages to Recvr
//
class MockChannel : public IMockChannel
{
public:
	MockChannel(ISendr* pSendr, IRecvr* pRecvr);
	void start();
	void stop();
private:
	std::thread thread_;
	ISendr* pISendr_;
	IRecvr* pIRecvr_;
	bool stop_ = false;
	std::string processmessage(Message);
	std::string clientcallupload(MsgClient&,std::string,std::vector<std::string>);
	std::string clientcalldownload(MsgClient&,std::string,std::string);
	std::string clientcallGF(MsgClient&,std::string);
	std::string clientcallGIF(MsgClient&,std::string);
	std::string clientcallIIS(MsgClient&,std::string);
	std::string clientcallLAZY(MsgClient&,std::string,std::string);
	std::string clientcallPublish(MsgClient&,std::string);
	std::string clientcalldelete(MsgClient&,std::string,std::string);
};
//split string with delimiter
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
//process message
std::string MockChannel::processmessage(Message msg)
{
	MsgClient c2("./Client1Repo/", "8081");
	int pos;
	pos = msg.find_first_of("/");
	std::string key = msg.substr(0, pos);
	std::string values = msg.substr(pos + 1, msg.size());
	pos = values.find_first_of("/");
	std::string catname = values.substr(0, pos);
	std::string names = values.substr(pos + 1, values.size());
	std::vector<std::string> fn = splitstring(names);
	std::string callbackmessage = "";
	if (key == "GetFiles")
	{
		callbackmessage = clientcallGF(c2, catname);
		callbackmessage += "::gtf";
		return callbackmessage;}
	else if (key == "GIFL")
	{
		callbackmessage = clientcallGIF(c2, catname);
		return callbackmessage;}
	else if (key == "Upload")
		{
		clientcallupload(c2, catname, fn);
		}
	else if (key == "Download" && !catname.empty() &&!fn.empty())
		{
		clientcalldownload(c2,catname, names);
			}
	else if (key == "Lazy" && !catname.empty() && !fn.empty())
		{
		clientcallLAZY(c2, catname, names);
			}
	else if (key == "Publish")
		{
		clientcallPublish(c2, catname);
			}
	else if (key == "Delete")
		{
		clientcalldelete(c2,catname, names);
	}
	else if (key == "IIS")
		{
		clientcallIIS(c2, names);
	}
	return callbackmessage;}

std::string MockChannel::clientcallIIS(MsgClient &c,std::string names)
{
	std::thread t1([&]() {c.SetupIIS(names); });
	t1.join();
	return "";
}
std::string MockChannel::clientcallPublish(MsgClient &c, std::string cat)
{
	std::thread t1([&]() {c.SendPublishMessage(cat); });
	t1.join();
	return "";
}
std::string MockChannel::clientcalldelete(MsgClient &c,std::string catn, std::string names)
{
	std::thread t1([&]() {c.SendDeletemsg(catn,names); });
	t1.join();
	return "";
}
std::string MockChannel::clientcallupload(MsgClient &c,std::string catn, std::vector<std::string> names)
{
	std::thread t1([&]() {c.upload(catn,names); });
	t1.join();
	return "";
}
std::string MockChannel::clientcalldownload (MsgClient &c, std::string catn, std::string names)
{
	std::thread t1([&]() {c.downloadFiles(catn,names); });
	t1.join();
	return "";
}
std::string MockChannel::clientcallLAZY(MsgClient &c, std::string catn, std::string names)
{
	std::thread t1([&]() {c.LazyDL(catn,names); });
	t1.join();
	return "";
}std::string MockChannel::clientcallGF(MsgClient &c, std::string catn)
{
	std::string res;
	std::thread t1([&]() {res=c.ReqFiles(catn); });
	t1.join();
	return res;
}std::string MockChannel::clientcallGIF(MsgClient &c, std::string names)
{
	std::string res;
	std::thread t1([&]() {res=c.ReqIFL(names); });
	t1.join();
	return res;
}
//----< pass pointers to Sender and Receiver >-------------------------------

MockChannel::MockChannel(ISendr* pSendr, IRecvr* pRecvr) : pISendr_(pSendr), pIRecvr_(pRecvr) {}

//----< creates thread to read from sendQ and echo back to the recvQ >-------

void MockChannel::start()
{
	//MsgClient c1;
	std::cout << "\n  MockChannel starting up";
	thread_ = std::thread(
		[this] {
		Sendr* pSendr = dynamic_cast<Sendr*>(pISendr_);
		Recvr* pRecvr = dynamic_cast<Recvr*>(pIRecvr_);
		if (pSendr == nullptr || pRecvr == nullptr)
		{
			std::cout << "\n  failed to start Mock Channel\n\n";
			return;
		}
		BQueue& sendQ = pSendr->queue();
		BQueue& recvQ = pRecvr->queue();
		while (!stop_)
		{
			std::cout << "\n  channel deQing message";
			Message msg = sendQ.deQ();  // will block here so send quit message when stopping
			std::cout << "\n  channel enQing message";
			std::string cb=processmessage(msg);
			if (cb != "")
				recvQ.enQ(cb);
			
		}
		std::cout << "\n  Server stopping\n\n";
	});
}
//----< signal server thread to stop >---------------------------------------

void MockChannel::stop() { stop_ = true; }

//----< factory functions >--------------------------------------------------

ISendr* ObjectFactory::createSendr() { return new Sendr; }

IRecvr* ObjectFactory::createRecvr() { return new Recvr; }

IMockChannel* ObjectFactory::createMockChannel(ISendr* pISendr, IRecvr* pIRecvr)
{
	return new MockChannel(pISendr, pIRecvr);
}

#ifdef TEST_MOCKCHANNEL

//----< test stub >----------------------------------------------------------

int main()
{
	ObjectFactory objFact;
	ISendr* pSendr = objFact.createSendr();
	IRecvr* pRecvr = objFact.createRecvr();
	IMockChannel* pMockChannel = objFact.createMockChannel(pSendr, pRecvr);
	pMockChannel->start();
	pSendr->postMessage("Hello World");
	pSendr->postMessage("CSE687 - Object Oriented Design");
	Message msg = pRecvr->getMessage();
	std::cout << "\n  received message = \"" << msg << "\"";
	msg = pRecvr->getMessage();
	std::cout << "\n  received message = \"" << msg << "\"";
	pSendr->postMessage("stopping");
	msg = pRecvr->getMessage();
	std::cout << "\n  received message = \"" << msg << "\"";
	pMockChannel->stop();
	pSendr->postMessage("quit");
	std::cin.get();
}
#endif
