#pragma once
#ifndef HELPERCLASS_H
#define HELPERCLASS_H



class HelperClass
{
public:
	HelperClass();
	~HelperClass();

	template<typename t>
	static t ReadLine(std::string question, t arg);

	static std::vector<char> HelperClass::stringToVector(std::string s);
	static void HelperClass::printl(std::string s);
	static void print(std::string s);
	static std::string fileExtensionSeeker(std::string filename);
};


template<typename t>
t HelperClass::ReadLine(std::string question, t arg)
{
	std::cout << question << std::endl;
	std::cin >> arg;
	return arg;
}

#endif