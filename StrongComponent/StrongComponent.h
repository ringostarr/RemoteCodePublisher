/////////////////////////////////////////////////////////////////////
// StrongComponent.h -find strong components                       //
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
Define Node class for graph
Find out strongly connected components via SSC class
Public Interface:
=================
Class node
-Addedge(node* x) add a new edge between current node and node x
-node() - default constructor
Class SSC
-int minimum() - find minimum between two numbers
-void strongconnect() - find out strong components and make xml tags from files
-void makegraphandSCcomponents() - populate graph and initiate strongconnect()
-int getnodefromlist() - gets index of node from a list
Build Process:
==============
Required files
- StrongComponent.h , StrongComponent.cpp
Build commands (either one)
- devenv oodp26.sln
- cl /EHsc /test_ssc StrongComponent.cpp /link setargv.obj

Maintenance History:
====================
ver1.0 : Initial commit


*/

#include <iostream>
#include<fstream>
#include <stack>

#include <vector>
//#include "../CppProperties/CppProperties.h"
#include<algorithm>
#include "XmlWriter.h"
#include "../NoSqlDb/NoSqlDb.h"
#include "../FileMgr/FileMgr.h"
#include "../FileMgr/IFileMgr.h"
class node
{
public:
	int index;
	int lowlink;
	int visited;
	std::string name;
	std::vector<node*> adjlist;
	//default constructor
	node()
	{
		visited = 0;
		index = 0;
	}
	//add new edge 
	void addedge(node* b)
	{
		adjlist.push_back(b);
	}


};

class SSC {
private:
	XmlWriter xwr;
public:

	//default constructor
	SSC()
	{

	}
	//finds and returns minimum of two numbers
	int minimum(int a, int b)
	{
		if (a >= b)
			return b;
		else
			return a;
	}
	//perform tarjans and create xml tags
	void strongconnect(node* nn, int index, std::stack<node*>*S)
	{
		node*  next;
		nn->index = index;
		nn->lowlink = index;
		index++;
		S->push(nn);
		nn->visited = 1;
		for (size_t i = 0; i < nn->adjlist.size(); i++)
		{
			next = nn->adjlist[i];
			if (next->visited == 0) {
				strongconnect(next, index, S);
				nn->lowlink = minimum(nn->lowlink, next->lowlink);
			}
			else if (next->visited == 1) {
				nn->lowlink = minimum(nn->lowlink, next->index);
			}
		}
		if (nn->lowlink == nn->index) {
			//printf("\n scc\n");
			xwr.start("Component");
			//xwr.indent();
			while (1) {
				node*  temp = S->top();
				S->pop();
				xwr.start("File");
				temp->name.insert(0, "\n");
				xwr.addBody(temp->name.append("\n").c_str());
				temp->visited = 2;
				xwr.end();
				//xwr.end();
				if (temp == nn) break;

			}
			xwr.end();
		}


	}
	//gets node from vector
	node getnodefromlist(std::vector<node> vec, std::string name)
	{
		for (size_t i = 0; i < vec.size(); i++)
		{
			if (vec[i].name == name)
			{
				return vec[i];
			}
		}
	}
	//search in db for specific key
	int searchfor(std::vector<node> db, std::string key)
	{
		//std::vector<std::string> keys = db.keys();
		int i = 0;
		for (size_t i = 0; i<db.size(); i++)
		{
			if (db[i].name == key)
				break;


		}
		return i;
	}
	//create graph from db and start SSC creation function
	void makegraphandSCcomponents(NoSqlDb<std::string> db)
	{
		std::vector<node*>v;
		std::vector<node> fv;
		std::vector<std::string> keys = db.keys();
		for (std::string key : keys)
		{
			node n;
			Element<std::string> ele;
			ele = db.value(key).ret();
			n.name = ele.name.getValue();
			fv.push_back(n);
		}
		int i = 0;
		for (std::string key : keys)
		{
			Element<std::string> el;
			el = db.value(key).ret();
			std::vector<std::string> ks;
			for (size_t j = 0; j < el.childrel.size(); j++)
			{
				int k = searchfor(fv, el.childrel[j]);
				fv[i].addedge(&fv[j]);
			}
			i++;
		}
		for (size_t p = 0; p < fv.size(); p++)
		{
			v.push_back(&fv[p]);
		}

		xwr.start("StronglyConnectedComponents");
		int index = 1;
		std::stack<node*>* S = new std::stack<node*>;
		for (size_t i = 0; i < v.size(); i++)
		{
			if (v[i]->visited == 0) {

				strongconnect(v[i], index, S);
			}
		}
		xwr.end();
		std::ofstream out;
		out.open("SSC.xml");
		out << xwr.xml().c_str();
		out.close();
		std::cout << "\n---StronglyConnected Components saved in:" << FileSystem::Path::getFullFileSpec("SSC.xml");
	}
};
