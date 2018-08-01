#include "Codepublisher.h"
#ifdef TEST_CODEPUBLISHER
int main()
{
	codepublisher cp;
	NoSqlDb<std::string> testdb;
	Element<std::string> ele;
	std::string filepath = "C:\\Users\\Ringo\\Documents\\Visual Studio 2017\\Projects\\oodp26\\DependencyAnalysis\\test.h";

	ele.name = filepath;
	ele.data = "some data";
	std::string ch = filepath + ".htm";
	ele.childrel.push_back(ch);
	testdb.save(ele.name, ele);
	cp.setdb(testdb);
	cp.htmlinit(ch);
	cp.adddep(ch);
	/*----Using Semi Exp----
	//Scanner::Toker toker;
	//std::fstream in(filepath);
	//if (!in.good())
	//{
	//	std::cout << "could not open file";
	//}
	//else
	//{
	//	std::cout << "\nProcessing \n";
	//}
	//toker.attach(&in);
	//cp.wrpretag(ch);
	//Scanner::SemiExp se(&toker);
	//while (se.get())
	//{
	//	std::string chck = se.show();
	//	if (chck.find("{") != std::string::npos)
	//		cp.detailstagst(ch, chck);
	//	else if (chck.find("}") != std::string::npos)
	//	{
	//
	//		cp.detailstagend(ch);
	//		cp.writestr(ch, chck);
	//	}
	//	else
	//		cp.writestr(ch, chck);

	//}
	//cp.wrpretagend(ch);
	//cp.endbody(ch);
	//cp.endhtml(ch);
	*/
	cp.wrpretag(ch);
	std::stringstream buffer;
	std::ifstream input(filepath);
	//buffer << input.rdbuf();*/
	for (std::string line; std::getline(input, line);)
	{
		if (line.find("{") != std::string::npos)
			cp.detailstagst(ch, line);
		else if (line.find("}") != std::string::npos)
		{
			cp.detailstagend(ch);
			cp.writestr(ch, line);
		}
		else
			cp.writestr(ch, line);
	}
	cp.wrpretagend(ch);
	cp.endbody(ch);
	cp.endhtml(ch);
}
#endif