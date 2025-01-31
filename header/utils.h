#ifndef UTILS_H
#define UTILS_H 

#include <iostream>
#include <basetsd.h>

class UtilsClass
{
public:
	std::string ExtractNameByPath(std::string path);

	std::string GetTimeCurrent(std::string s);

	std::string Logger(std::string logMsg);

private:
	std::string defaultPath;
};

#endif