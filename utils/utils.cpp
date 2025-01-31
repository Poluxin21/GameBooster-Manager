#include "../header/utils.h"
#include "../header/memory_manager.h"
#include <chrono>
#include <iostream>
#include <fstream>

std::string UtilsClass::ExtractNameByPath(std::string path)
{
	std::string allpath = path;
	std::string result = allpath.substr(allpath.find_last_of("/\\") + 1);
	return result;
}

std::string UtilsClass::GetTimeCurrent(std::string s)
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
	return std::string(buf);

}


std::string UtilsClass::Logger(std::string logMsg)
{

	defaultPath = "C:/turatlas/log_" + GetTimeCurrent("data") + ".txt";
	std::string now = GetTimeCurrent("now");
	std::ofstream ofs(defaultPath.c_str(), std::ios_base::out | std::ios_base::app);

	if (!ofs)
	{
		return "Erro de sistema";
	}

	ofs << now << '\t' << logMsg << '\n';
	ofs.close();

}
