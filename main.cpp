#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <ctime>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <thread>

#include "TcpListener.h"
#include "HelperClass.h"

#define INVALID_GET "invalid get request"

std::string ProjectFolder = "site2\\";
std::string StartDocument = "index.html";



std::vector<char> getFile(std::string path);

std::vector<char> parseGet(char buffer[], char* type);

//Function passed to tcplistener-> handles client socket
void ListenFunction(SOCKET s);




std::vector<char> getFile(std::string path)
{
	if (path.length() < 2)
	{
		return HelperClass::stringToVector(ProjectFolder + StartDocument);
	}
	else
	{
		std::string::iterator end_pos = std::remove(path.begin(), path.end(), ' ');
		path.erase(end_pos, path.end());

		std::replace(path.begin(), path.end(), '/', '\\');
		std::ifstream t(ProjectFolder + path, std::ios::binary);
		
		t.seekg(0, std::ios::end);
		std::streampos size = t.tellg();
		t.seekg(0, std::ios::beg);

		try
		{
			std::vector<char> fileContents((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
			return fileContents;
		}
		catch (const std::length_error& le)
		{
			HelperClass::printl("ERROR DOWN HERE");
			HelperClass::print(path);
			HelperClass::printl(le.what());
			HelperClass::printl(std::to_string(size));
			return HelperClass::stringToVector("error");
		}


	}

}


std::vector<char> parseGet(char buffer[], char& type)
{
	std::string fileGet = "";
	std::vector<char> fileContent;

	if (buffer == nullptr)
	{
		return HelperClass::stringToVector(INVALID_GET);
	}

	if (buffer[0] == 'G' && buffer[1] == 'E' && buffer[2] == 'T')
	{

		int stop = 0;
		int index = 3;
		while (stop == 0)
		{
			if (buffer[index + 1] == 'H' && buffer[index + 2] == 'T' && buffer[index + 3] == 'T' && buffer[index + 4] == 'P')
			{
				stop = 1;
			}

			if (buffer[index + 1] == '/' && buffer[index + 2] == ' ' && buffer[index + 3] == 'H' && buffer[index + 4] == 'T' && buffer[index + 5] == 'T' && buffer[index + 6] == 'P')
			{
				fileGet = StartDocument;
				break;
			}

			else
			{
				fileGet.push_back(buffer[index]);
			}
			index++;
		}

		std::string fileext = HelperClass::fileExtensionSeeker(fileGet);
		fileext = fileext.substr(0, fileext.length() - 1);

		if (fileext == "ico" || fileext == "jpg" || fileext == "png" || fileext == "jpeg" || fileext == "svg")
		{
			type = '1';
		}
		if (fileext == "css")
		{
			type = '2';
		}
		if (fileext == "js")
		{
			type = '3';
		}
		if (fileext == "html")
		{
			type = '0';
		}
		std::string woffType = ".woff";
		std::string ttfType = ".ttf";

		if (fileext.find(ttfType) != std::string::npos || fileext.find(woffType) != std::string::npos)
		{
			type = '4';
		}

		//clean empty chars
		std::string::iterator end_pos = std::remove(fileGet.begin(), fileGet.end(), ' ');
		fileGet.erase(end_pos, fileGet.end());

		HelperClass::printl(fileGet);

		//read the file requested
		fileContent = getFile(fileGet);
		return fileContent;
	}
	else
	{
		return HelperClass::stringToVector("there is no file");
	}


}

void ListenFunction(SOCKET s)
{

	char buf[1024];
	recv(s, buf, 2048, 0);

	char type;

	time_t now = time(0);
	std::string datetimeNow = asctime(localtime(&now));

	std::string message_fmt;
	std::vector<char> imageBuffer = parseGet(buf, type);


	std::string sizeS = std::to_string(imageBuffer.size());

	if (type == '0')
	{
		message_fmt = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + sizeS + " \r\n\r\n";
	}
	if (type == '1')
	{
		message_fmt = "HTTP/1.1 200 OK\r\nContent-Type: application/octet-stream\r\nContent-Length: " + sizeS + " \r\n\r\n";
	}
	if (type == '2')
	{
		message_fmt = "HTTP/1.1 200 OK\r\nContent-Type: text/css\r\nContent-Length: " + sizeS + " \r\n\r\n";
	}
	if (type == '3')
	{
		message_fmt = "HTTP/1.1 200 OK\r\nContent-Type: application/javascript\r\nContent-Length: " + sizeS + " \r\n\r\n";
	}
	if (type == '4')
	{
		message_fmt = "HTTP/1.1 200 OK\r\nContent-Type: font/ttf\r\nContent-Length: " + sizeS + " \r\n\r\n";
	}

	send(s, message_fmt.c_str(), message_fmt.length(), 0);
	send(s, imageBuffer.data(), imageBuffer.size(), 0);

}





void main(void)
{

	ProjectFolder = HelperClass::ReadLine("Give working folder in form folder\\ <-- 2 slashes ", ProjectFolder);


	TcpListener tlisten = TcpListener(1024, "80");


	if (!tlisten.Initialize())
	{
		HelperClass::printl("Error with initializing socket closing...");
	}
	

	tlisten.Listen(ListenFunction);

	tlisten.ShutDown();

	getchar();
}