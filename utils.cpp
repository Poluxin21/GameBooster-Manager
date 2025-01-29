#include "utils.h"
#include "memory_manager.h"
#include <chrono>
#include <iostream>
#include <fstream>
using namespace std;

string UtilsClass::ExtractNameByPath(string path)
{
	string allpath = path;
	string result = allpath.substr(allpath.find_last_of("/\\") + 1);
	return result;
}

string UtilsClass::GetTimeCurrent(string s)
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	localtime_s(&tstruct, &now);
	if (s == "now")
	{
		strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
	}
	else if (s == "date")
	{
		strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
	}
	return string(buf);

}


string UtilsClass::Logger(string logMsg)
{

	defaultPath = "C:/turatlas/log_" + GetTimeCurrent("data") + ".txt";
	string now = GetTimeCurrent("now");
	ofstream ofs(defaultPath.c_str(), ios_base::out | ios_base::app);

	if (!ofs)
	{
		return "Erro de sistema";
	}

	ofs << now << '\t' << logMsg << '\n';
	ofs.close();

}
