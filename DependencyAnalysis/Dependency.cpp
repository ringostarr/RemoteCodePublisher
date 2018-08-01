//#include <iostream>#define  _CRT_SECURE_NO_WARNINGS
#define  _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include "Dependency.h"
#include "../FileMgr/FileSystem.h"
#ifdef TT_DEP

// TT_DEP

int main()
{
	Value v;
	std::vector<std::string> p;
	v.setvalue("abc", "asdasd", "scope");
	TypeTable tx;
	tx.setvalue("key1", v);
	tx.displayTab();
	Dependency d;

	std::string y = FileSystem::Path::getFullFileSpec("Actionsandrules.h");
	std::vector<std::string> yy;
	yy.push_back(y);
	d.DoDepAnalysis(tx, yy, "xml.xml");

}
#endif