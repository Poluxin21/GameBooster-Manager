#ifndef UTILS_H
#define UTILS_H 

#include <iostream>
#include <basetsd.h>

class UtilsClass
{
public:
	std::string ExtractNameByPath(const std::string& path);

	std::string GetTimeCurrent(const std::string& format);

	void Logger(const std::string& logMsg);

private:
	std::string defaultPath;
};

#endif