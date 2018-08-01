#pragma once
/////////////////////////////////////////////////////////////////////
// TypeTable.h -define TypeTable                                   //
//  Akshay,                                                        //
//  708,S beech Street #1 ,Syracuse .                              //
//  Akshay@syr.edu                                                 //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   CSE687 Pr1, Spring 2017                         //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
Defines the components of TypeTable

Public Interface:
=================
Class Value - assigns to Values
class TypeTable - functions to populate and display typetable
Build Process:
==============
Required files
- TypeTable.h , TypeTable.cpp
Build commands (either one)
- devenv oodp26.sln
- cl /EHsc /test_tt TypeTable.cpp /link setargv.obj

Maintenance History:
====================
ver1.0 : Initial commit


*/
#include <unordered_map>
#include <vector>

class Value
{
private:
	std::string filename;
	std::string nsname;
	std::string type;
public:
	//Value();
	//~Value();
	void setvalue(std::string, std::string, std::string);
	std::string getfn();
	std::string getns();
	std::string getyp();
};

class TypeTable
{
private:
	std::unordered_map<std::string, std::vector<Value>> Typtab;
	using Item = std::pair<std::string, std::vector<Value>>;
public:

	using Keys = std::vector<std::string>;

	//~TypeTable();
	std::vector<std::string> Key()
	{
		Keys ks;
		for (Item item : Typtab)
		{
			ks.push_back(item.first);
		}
		return ks;
	}


	void setvalue(std::string k, Value v);
	std::unordered_map<std::string, std::vector< Value>> gettypetable();
	//find key in typetable
	std::vector<Value> find(std::string key)
	{
		std::unordered_map<std::string, std::vector<Value>>::const_iterator got = Typtab.find(key);
		if (got == Typtab.end())
		{
			return got->second;
		}
		else
		{
			return got->second;


		}
	}
	void displayTab();
};