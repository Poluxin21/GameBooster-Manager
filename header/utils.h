#ifndef UTILS_H
#define UTILS_H 

#include <iostream>
#include <basetsd.h>

using namespace std;

class UtilsClass
{
public:
	string ExtractNameByPath(string path);

	string GetTimeCurrent(string s);

	string Logger(string logMsg);

private:
	string defaultPath;
};

#endif