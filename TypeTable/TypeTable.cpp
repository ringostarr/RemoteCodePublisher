#include "TypeTable.h"
#include<iostream>
#include<string>
//sets value 
void Value::setvalue(std::string path, std::string namesp, std::string typenamex)
{
	filename = path;
	nsname = namesp;
	type = typenamex;
}
//returns filepath
std::string Value::getfn()
{
	return filename;
}
//returns namespace
std::string Value::getns()
{
	return nsname;
}
//returns type
std::string Value::getyp()
{
	return type;
}
//set value of preexisting
void TypeTable::setvalue(std::string k, Value v)
{
	//if (Typtab.find(k) == Typtab.end())

	Typtab[k].push_back(v);

}
//return map
std::unordered_map<std::string, std::vector< Value>> TypeTable::gettypetable()// gettypetable();
{
	return Typtab;
}
//display function
void TypeTable::displayTab()
{
	std::cout << "\n\n===================Displaying TypeTable Req 4 ====================================\n\n";
	std::cout << "Name       Type          Path                                                           Namespace\n";
	std::cout << "-----------------------------------------------------------------------------------------------------\n";
	Keys ks;
	ks = Key();
	for (std::string k : ks)
	{
		for (size_t i = 0; i<Typtab[k].size(); i++)
		{
			std::cout << "\n" << k;
			std::string fn = Typtab[k][i].getfn();
			std::string nn = Typtab[k][i].getns();
			std::string tn = Typtab[k][i].getyp();
			std::cout << "---" << tn << "---" << fn << "---" << nn;
			std::cout << "\n";
		}
	}
}
#ifdef  TEST_TT

//  TEST_TT

int main()
{
	Value v;
	v.setvalue("abc", { "asd","kkk" });
	TypeTable tx;
	tx.setvalue("key1", v);
	tx.display();
}
#endif