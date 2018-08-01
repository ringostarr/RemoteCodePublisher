// DepAnal
#define _CRT_SECURE_NO_WARNINGS
#include "DepAnal.h"
using namespace CodeAnalysis;


int main()
{
	
	TypeAnal x;
	std::vector<std::string> fp;
	std::vector<std::string> y;
	y.push_back( "Actionsandrules.h");
	//fp.push_back(y);
	x.doTypeAnal(y);

}

