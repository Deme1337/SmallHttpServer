#include <iostream>
#include <string>
#include <vector>

#include "HelperClass.h"



HelperClass::HelperClass()
{
}


HelperClass::~HelperClass()
{
}

void HelperClass::HelperClass::printl(std::string s)
{
	std::cout << s << std::endl;
}

void HelperClass::print(std::string s)
{
	std::cout << s;
}


std::vector<char> HelperClass::HelperClass::stringToVector(std::string s)
{
	std::vector<char> vec(s.begin(), s.end());

	return vec;
}

std::string HelperClass::fileExtensionSeeker(std::string filename)
{
	std::string::size_type idx;

	idx = filename.rfind('.');

	if (idx != std::string::npos)
	{
		return filename.substr(idx + 1);
	}
	else
	{
		return "none";
	}
}

