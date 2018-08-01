#pragma once
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4006)

/////////////////////////////////////////////////////////////////////
// DepAnal.h -Populate type table from AST                      //
//  Akshay,
//  708,S beech Street #1 ,Syracuse .
//  Akshay@syr.edu
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   CSE687 Pr1, Spring 2017
//	Reference: Jim Fawcett, CSE687 - Object Oriented Design, Spring 2017  //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
Start TypeAnalysis by populating typetable from AST

Public Interface:
=================
DFS() - Performs recursive depth first search
dotypeanal() - STarts typeanalysis and returns typetable
Build Process:
==============
Required files
- DepAnal.h , DepAnal.cpp , TypeTable.h , TypeTable.cpp , ActionsAndrules.h , ActionsAndrules.cpp
Build commands (either one)
- devenv oodp26.sln
- cl /EHsc /TEST_DEPANAL depAnal.cpp /link setargv.obj

Maintenance History:
====================
ver1.0 : Initial commit
ver 1.1 : Changed to incorporate filespec

*/

#include "../Parser/ActionsAndRules.h"
#include "../TypeTable/TypeTable.h"
#include<stack>
#include<algorithm>
#include<vector>

namespace CodeAnalysis
{
	class TypeAnal
	{
	public:
		using SPtr = std::shared_ptr<ASTNode*>;

		TypeAnal();
		TypeTable doTypeAnal(std::vector<std::string> FilesPaths);
		TypeTable gettypetable()
		{
			return tt;
		}
	private:
		void DFS(ASTNode* pNode);
		AbstrSynTree& ASTref_;
		ScopeStack<ASTNode*> scopeStack_;
		Scanner::Toker& toker_;
		TypeTable tt;
		std::string parenttype;
		std::string paretnname;
		std::stack<ASTNode*> st;
	};

	inline TypeAnal::TypeAnal() :
		ASTref_(Repository::getInstance()->AST()),
		scopeStack_(Repository::getInstance()->scopeStack()),
		toker_(*(Repository::getInstance()->Toker()))
	{
	}
	// Depth first search of astnode
	inline void TypeAnal::DFS(ASTNode* pNode)
	{

		static std::string path = "";
		if (pNode->path_ != path)
		{
			std::cout << "\n    -- " << pNode->path_ << "\\" << pNode->package_;
			path = pNode->path_;
		}

		if (pNode->name_ == "Value")
		{
			std::cout << "";
		}
		std::string key = pNode->path_;
		if (pNode->name_ != "none" && pNode->type_ != "anonymous") {
			std::cout << "\n  " << pNode->name_;
			std::cout << ", " << pNode->type_;
			if (pNode->type_ != "control") {
				Value v;
				v.setvalue(pNode->path_, paretnname, pNode->type_);
				if ((pNode->type_ == "function" || pNode->type_ == "lambda") && parenttype != "class" && pNode->name_ != "main")
					tt.setvalue(pNode->name_, v);

				else if (pNode->type_ != "function" &&pNode->type_ != "lambda")
					tt.setvalue(pNode->name_, v);
			}
		}
		for (auto pChild : pNode->children_)
		{
			parenttype = pNode->type_;
			paretnname = pNode->name_;

			//st.push(pNode);
			DFS(pChild);

		}

	}
	//start Type Analysis
	inline TypeTable TypeAnal::doTypeAnal(std::vector<std::string> FilesPaths)
	{

		ASTNode* pRoot = ASTref_.root();
		parenttype = pRoot->type_;
		paretnname = pRoot->name_;
		DFS(pRoot);
		std::cout << "\n-----Requirement 1 & 2: Please Check Code using visual studio 2015\n";
		std::cout << "\n -------------------TypeTable Created--------------------\n";
		//tt.displayTab();
		return tt;

	}
}
