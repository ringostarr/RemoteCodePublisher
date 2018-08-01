/////////////////////////////////////////////////////////////////////
// Convert.cpp - Implement serialization to and from strings       //
//                                                                 //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2017       //
/////////////////////////////////////////////////////////////////////

#include "Convert.h"
#include "..\Utilities\Utilities.h"
#include <iostream>
#include <functional>

/////////////////////////////////////////////////////////////////////
// Widget class - shows user defined types can be converted
// - must have operator<< and operator>>

/////////////////////////////////////////////////////////////////////
// Demo code

int main()
{

	std::string intString = Convert<int>::toString(42);
	std::cout << "\n  conversion of integer: " << intString;
	std::cout << "\n  retrieving integer:    " << Convert<int>::fromString(intString);

	std::string dblString = Convert<double>::toString(3.1415927);
	std::cout << "\n  conversion of double:  " << dblString;
	std::cout << "\n  retrieving double:     " << Convert<double>::fromString(dblString);
	//putLine();



}