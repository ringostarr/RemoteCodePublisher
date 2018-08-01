/////////////////////////////////////////////////////////////////////
// CodePublisher.h - Pushlish Code in HTML format                  //
//  Akshay,
//  708,S beech Street #1 ,Syracuse .
//  Akshay@syr.edu
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   CSE687 Pr1, Spring 2017
//	                                                                //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
Takes vector of filepaths and Database as input
and generates html files with collapsable content.
Public Interface:
=================
Class codepublisher
-codepublisher() - default constructor
-setdb()  - sets db
-setpath() - set path of particular file
-void procfiles() - process vector of files
-retTime() - returns current time
-htmlinit() - initial function to write <html> and head tags
-endhtml() - ends html tag
-adddep() - adds dependecy link with <a href> tag
-replaceAll() - replaces < and > with &lt; and &gt;
-writestr() - writes string to html file
-CreateBody() - Start body tag
-wrpretag() - start pre tag
-detailstagst() - html5 details tag start
-detailstagend() - html5 details tag end
-wrpretagend() - end pre tag
-CreateToCPage() - creates table of contents page
-ConvertHtml() - takes file vector as input and creates html for all files in vector

Build Process:
==============
Required files
- codepublisher.h , codepublisher.cpp
Build commands (either one)
- devenv oodp26.sln
- cl /EHsc /test_ssc codepublisher.cpp /link setargv.obj

Maintenance History:
====================
ver1.0 :04/01/2017: Initial commit


*/
#pragma once
#include "../DependencyAnalysis/Dependency.h"
#include <Windows.h>
#include <shellapi.h>
class codepublisher
{
private:
	NoSqlDb<std::string> ndb;
	std::string filepath;
	std::vector<std::string> filesrec;
public:
	//Default constructor
	codepublisher()
	{

	}
	//parameterized constructor
	codepublisher(NoSqlDb<std::string> &db)
	{
		ndb = db;
	}
	//sets internal db
	void setdb(NoSqlDb<std::string> db)
	{
		ndb = db;
	}
	//set path
	void setpath(std::string fp)
	{
		filepath = fp;
	}
	//process files and put them in filesrec vector
	void procfiles(std::vector<std::string> files)
	{
		for (std::string file : files)
		{
			std::string temp = file + ".htm";
			filesrec.push_back(temp);
		}
	}
	//returns current time
	std::string retTime()
	{
		time_t now = time(NULL);
		struct tm* timex = std::localtime(&now);
		char buffer[32];
		std::strftime(buffer, 32, "%d:%m:%Y %H:%M:%S", timex);
		return buffer;
	}
	//intial html tag
	void htmlinit(std::string fp)
	{
		std::ofstream myfile;
		myfile.open(fp);
		myfile << "<!DOCTYPE html><html><head>";
		myfile << std::endl;
		myfile << "<!-------------HTML Prologue------------!>";
		myfile << std::endl;
		myfile << "<!---Published By: Akshay , 708 S Beech Street , Syracuse .--!>";
		myfile << std::endl;
		myfile << "<!----------------------(315)-289-0056----------------------!>";
		myfile << std::endl;
		std::string pname;
		pname = fp.substr(0, fp.size() - 4);
		myfile << "<!----Package Name:" << pname << "-----!>";
		myfile << std::endl;
		std::string t;
		t = retTime();
		myfile << "<!-------Published on :" << t << "-----!>";
		myfile << std::endl;
		myfile << "<link rel=""stylesheet ""type=""text/css ""href=\"../Common/Stylesheet.css\"""/>";
		myfile << "</head>";
		myfile.close();
	}
	//end html tag
	void endhtml(std::string fp)
	{
		std::ofstream myfile;
		myfile.open(fp, std::ios_base::app);
		myfile << "</html>";
		myfile.close();
	}
	//add dependecies to html file
	void adddep(std::string fp)
	{
		std::vector<std::string> keys = ndb.keys();
		std::vector<std::string> crel;
		Element<std::string> ele;
		int found = fp.find(".htm");
		std::string dbval = fp.substr(0, found);
		ele = ndb.value(dbval).ret();
		for (std::string child : ele.childrel)
		{
			crel.push_back(child);
		}
		std::ofstream myfile;
		myfile.open(fp, std::ios_base::app);
		myfile << " <div class = ""indent""><h4>Dependencies:</h4>";
		for (std::string ch : ele.childrel)
		{
			myfile << "<a href = \"" << FileSystem::Path::getName(FileSystem::Path::getFullFileSpec(ch)) << ".htm\"" << ">" << FileSystem::Path::getName(FileSystem::Path::getFullFileSpec(ch));
			myfile << "</a>";
			myfile << "<br>";
		}
		myfile << " </div>";
		myfile << "</hr>";
		myfile.close();
	}
	//replace < and >
	std::string replaceAll(std::string source, std::string from, std::string to)
	{
		std::string newString;
		newString.reserve(source.length());
		std::string::size_type lastPos = 0;
		std::string::size_type findPos;

		while (std::string::npos != (findPos = source.find(from, lastPos)))
		{
			newString.append(source, lastPos, findPos - lastPos);
			newString += to;
			lastPos = findPos + from.length();
		}
		newString += source.substr(lastPos);
		return newString;
	}
	//writes string to html file
	void writestr(std::string fp, std::string str)
	{
		std::ofstream myfile;
		myfile.open(fp, std::ios_base::app);
		str = replaceAll(str, "<", "&lt;");
		str = replaceAll(str, ">", "&gt;");
		myfile << str;
		myfile << '\n';
		myfile.close();
	}
	//create <body> tag
	void createbody(std::string fp)
	{
		std::ofstream myfile;
		myfile.open(fp, std::ios_base::app);
		myfile << "<body>";
		myfile.close();
	}
	//create pre tag
	void wrpretag(std::string fp)
	{
		std::ofstream myfile;
		myfile.open(fp, std::ios_base::app);
		myfile << "<pre>";
		myfile.close();

	}
	//create details tag
	void detailstagst(std::string fp, std::string str)
	{
		std::ofstream myfile;
		myfile.open(fp, std::ios_base::app);
		myfile << "<details><summary>";
		str = replaceAll(str, "<", "&lt;");
		str = replaceAll(str, ">", "&gt;");
		myfile << str;
		myfile << "</summary>" << std::endl;
		myfile.close();
	}
	//end details tag
	void detailstagend(std::string fp)
	{
		std::ofstream myfile;
		myfile.open(fp, std::ios_base::app);
		myfile << "</details>";
		myfile.close();
	}
	//end pre tag
	void wrpretagend(std::string fp)
	{

		std::ofstream myfile;
		myfile.open(fp, std::ios_base::app);
		myfile << "</pre>";
		myfile.close();
	}
	//end body tag
	void endbody(std::string fp)
	{
		std::ofstream myfile;
		myfile.open(fp, std::ios_base::app);
		myfile << "</body>";
		myfile.close();
	}
	//create Table of Contents Page
	std::string CreateToCPage(std::vector<std::string> files,std::string name)
	{
		std::ofstream myfile;
		std::string x = name+".htm";
		myfile.open(x);
		myfile << "<!DOCTYPE html><html>";
		myfile << "<link rel=""stylesheet ""type=""text/css ""href=\"../Common/Stylesheet.css\"""/>";
		myfile << "<title> TABLE OF CONTENTS</title>";
		myfile << "<header><h1>TABLE OF CONTENTS</h1></header>";
		myfile << "<body>";
		for (std::string file : files)
		{
			myfile << "<a href = \"" << FileSystem::Path::getName(FileSystem::Path::getFullFileSpec(file)) << "\"" << ">" << FileSystem::Path::getName(FileSystem::Path::getFullFileSpec(file));
			myfile << "</a>";
			myfile << "<br>";
		}
		myfile << "</body>";
		myfile << "</html>";
		myfile.close();
		std::cout << "\n---------Created Table of Contents Page----------\n";
		//std::cout << "Page name is : MainPage.htm and is saved in root folder\n";
		return x;
	}
	//convert all files in vector to html file
	void convertHtml(std::vector<std::string> files,std::string fn)
	{
		std::cout << "\n-------Running on specified Path: root project folder--- Can change by changing command line arguments----------\n";
		std::cout << "===============================================================\n";
		std::cout << "\n------: started publishing ----------\n";
		std::cout << "\n------: Check Html file and its source \n";

		for (std::string file : files)
		{
			std::string ch = file + ".htm";
			std::cout << "\nPublishing file:" << file << std::endl;
			filesrec.push_back(ch);
			htmlinit(ch);
			adddep(ch);
			wrpretag(ch);
			std::stringstream buffer;
			std::ifstream input(file);
			for (std::string line; std::getline(input, line);)
			{
				if (line.find("{") != std::string::npos && line.find("}") == std::string::npos)
				{
					detailstagst(ch, line);
					continue;
				}
				else if (line.find("}") != std::string::npos && line.find("{") == std::string::npos)
				{
					detailstagend(ch);
					writestr(ch, line);
				}
				else
					writestr(ch, line);
			}
			wrpretagend(ch);
			endbody(ch);
			endhtml(ch);
		}
		std::string x = fn;
		x = x.substr(0, x.find(".xml"));
		std::string xl= CreateToCPage(filesrec,x);
		HWND parent = NULL;
		const char* url = xl.c_str();
		//ShellExecute(NULL, "open", app.c_str(), "MainPage.htm", NULL, SW_SHOWDEFAULT);
		//std::cout << "\n-----Opening MainPage.htm ----- Run using Google Chrome or Firefox for optimal result\n";
		//HINSTANCE result = ShellExecuteA(parent, NULL, url, NULL, NULL, SW_SHOWNORMAL);
	}
};