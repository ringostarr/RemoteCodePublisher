#pragma once
#define  _CRT_SECURE_NO_WARNINGS
/////////////////////////////////////////////////////////////////////
// Dependency.h - finds out dependcies in files                  //
//  Akshay,
//  708,S beech Street #1 ,Syracuse .
//  Akshay@syr.edu
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   CSE687 Pr1, Spring 2017
//	   //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
Creates a dependency list and stores it into the database for each file

Public Interface:
=================
class Dependency()
- string sgettime() - gets the current date time in string format
-DoDepAnalysis () - populates NoSqlDb with dependencies and returns db
Build Process:
==============
Required files
- Dependency.h , Dependency.cpp ,Tokenizer.h , Utilities.h.Tokenizer.cpp , Utilities.cpp
Build commands (either one)
- devenv oodp26.sln
- cl /EHsc /TEST_DEP Dependency.cpp /link setargv.obj

Maintenance History:
====================
ver1.0 : Initial commit

*/

#include<iostream>
#include<cctype>
#include<string>
#include<vector>
#include<unordered_set>
#include "../Tokenizer/Tokenizer.h"
//#include "../Utilities/Utilities.h"
#include "../TypeTable/TypeTable.h"
#include "../NoSqlDb/NoSqlDb.h"
//#include "Executive/Executive.h"
#include "../FileMgr/FileMgr.h"
#include "../FileMgr/IFileMgr.h"
#include "../SemiExp/SemiExp.h"
class Dependency
{
private:
	NoSqlDb<std::string> ndb;
	TypeTable typ;
	std::string currnamespace;
	std::unordered_set<std::string> nslist;
public:
	//default constructor
	Dependency()
	{

	}
	//parameterized constructor
	Dependency(TypeTable tt)
	{
		//ndb = db;
		typ = tt;

	}
	//get current time
	std::string sgettime()
	{
		time_t now = time(NULL);
		struct tm* timex = std::localtime(&now);
		char buffer[32];
		std::strftime(buffer, 32, "%d:%m:%Y %H:%M:%S", timex);
		return buffer;
	}
	//perform dependency analysis on filepaths using typetable tt and store result in xml file
	NoSqlDb<std::string> DoDepAnalysis(TypeTable tt, std::vector<std::string> FilesPaths, std::string xml)
	{
		NoSqlDb<std::string> db;
		for (auto item : FilesPaths)
		{
			nslist.insert("Global Namespace");
			Element<std::string> ele;
			try {
				std::ifstream in(FileSystem::Path::getFullFileSpec(item));
				if (!in.good()) {
					std::cout << "\nCould not open file " << item << "\n";
					return db;
				}
				Scanner::Toker toker;

				toker.returnComments();
				std::string exp;
				toker.attach(&in);
				do {
					std::string tok = toker.getTok();
					if (tok == "\n" || tok.find("//") == 0 || tok.find("/*") == 0)
						tok = "newline";
					else
					{
						std::vector<Value> v = tt.find(tok);
						if (!v.empty())
							for (size_t i = 0; i < v.size(); i++)
							{
								ele.name = item;
								ele.timeDate = sgettime();
								if (std::find(ele.childrel.begin(), ele.childrel.end(), v[i].getfn()) == ele.childrel.end() && ele.name.getValue() != v[i].getfn())
									ele.childrel.push_back(v[i].getfn());
							}
					}
				} while (in.good());
				db.save(ele.name, ele);
			}
			catch (std::logic_error& ex) {
				std::cout << ex.what();
			}
		}
		std::vector<std::string> keys = db.keys();
		std::cout << "\n===============================================================================================\n";
		std::cout << "----- Dependency Analysis Done ----\n";
		DBWrapper<std::string> dbw;
		dbw.save(xml, db);
		std::cout << "Database Saved at:" << FileSystem::Path::getFullFileSpec(xml);
		return db;
	}
};