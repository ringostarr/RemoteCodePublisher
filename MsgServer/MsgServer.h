#pragma once
/////////////////////////////////////////////////////////////////////
// MsgServer.cpp - Server Part of Project 4                        //
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
Acts as a receiver for TCP socket , read messages, files
Additionally is able to send msg back to requesting client

Public Interface:
=================
Class ClientHandler
-Operator() - msgs are read
-publish() - publishes specific category
-readMessage() - read a msg
-readFile() - read stream of file and save it at repository
Class MsgServer
-void sendingFile() - Send Files
-void LazySending() - find lazy files and send to client
-void getnsendfilenames() - gets files names from specific category
-void getnsendIfilenames() - gets independent file names from specific category
-void Setupiis() - setup IIS virtual directory
-makeMessage() - create a http message
-sendFile() - Send a file to client
-sendMessage() -Send message to client
-std::string toaddr - address where msg came from
-int port - port number
-void createnewindexpage() - creates indexpage.htm for IIS
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
#include "../MsgClient/Sockets.h"
#include "../ServerExecutive/Executive.h"
class MsgServer
{
public:
	
	using EndPoint = std::string;
	void sendingFile(std::string paths, std::string catn);
	void LazySending(std::string paths, std::string catn);
	MsgServer();
	void getnsendfilesname(std::string path);
	void getnsendIfilename(std::string path, std::string cat);
	void Setupiis(std::string path);
	std::string toaddr;
	void createnewindexpage(std::string);
	
	int port;

private:
	HttpMessage makeMessage(size_t n, const std::string& msgBody, const EndPoint& ep);
	void sendMessage(HttpMessage& msg, Socket& socket);
	bool sendFile(const std::string& fqname, Socket& socket);
	SocketConnecter si;
	SocketSystem ss;// si
};
