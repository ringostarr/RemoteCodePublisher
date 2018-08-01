#pragma once
/////////////////////////////////////////////////////////////////////
// MsgClient.h - Server Part of Project 4                        //
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
Sends TCP requests to server for publishing and communicates
with GUI to display messages
Public Interface:
=================
Class ServerHandler
-Operator() - msgs are read
-readMessage() - read a msg
-readFile() - read stream of file and save it at repository
Class MsgClient
void downloadFiles(std::string catname, std::string filenames)-download files
void upload(std::string,std::vector<std::string>) - upload files to category
void LazyDL(std::string, std::string)-LazyDownload function
std::string ReqIFL(std::string)- Independent file msg to server
void SendPublishMessage(std::string category)- send publish message to server
void SendDeletemsg(std::string catn, std::string names)-send delte msg to server
std::string ReqFiles(std::string)- send file name request to server
void SetupIIS(std::string path) - send iis setup message to server

Build Process:
==============
* Required Files :
*MsgClient.cpp, MsgServer.cpp
*   HttpMessage.h, HttpMessage.cpp
*   Cpp11 - BlockingQueue.h
*   Sockets.h, Sockets.cpp
*   FileSystem.h, FileSystem.cpp
*   Logger.h, Logger.cpp
*   Utilities.h, Utilities.cpp

Build commands
- devenv oodp4.sln


Maintenance History:
====================
ver2.0 : CHanged for compatibilty with project 4
ver1.0 : Initial commit
*/

#include "../HttpMessage/HttpMessage.h"
#include "Sockets.h"
#include "FileSystem.h"
#include "../Utilities/Utilities.h"
#include "../Logger/Logger.h"
#include <iostream>
#include <thread>

using Show = Logging::StaticLogger<1>;
using namespace Utilities;
using Utils = StringHelper;
/////////////////////////////////////////////////////////////////////
// MsgClient class
// - was created as a class so more than one instance could be 
//   run on child thread
//
class MsgClient
{
public:
	MsgClient();
	MsgClient(std::string, std::string);
	using EndPoint = std::string;
	void downloadFiles(std::string catname, std::string filenames);
	void upload(std::string,std::vector<std::string>);
	void LazyDL(std::string, std::string);
	std::string ReqIFL(std::string);
	void SendPublishMessage(std::string category);
	void SendDeletemsg(std::string catn, std::string names);
	std::string ReqFiles(std::string);
	void SetupIIS(std::string path);


private:
	HttpMessage makeMessage(size_t n, const std::string& msgBody, const EndPoint& ep);
	void sendMessage(HttpMessage& msg, Socket& socket);
	bool sendFile(const std::string& fqname, Socket& socket,std::string);
	SocketSystem ss;
	SocketConnecter si;
	std::string port;
	std::string repoPath;

	
};

class serverhandler
{
public:
	using EndPoint = std::string;
	
	serverhandler(BlockingQueue<HttpMessage>& msgQ) : msgQ_(msgQ) {}
	void operator()(Socket socket);
	EndPoint RepoPath;
	int myport;
private:
	bool connectionClosed_;
	HttpMessage readMessage(Socket& socket);
	bool readFile(const std::string& filename, size_t fileSize, Socket& socket);
	BlockingQueue<HttpMessage>& msgQ_;
	
	
};