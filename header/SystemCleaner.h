#ifndef SYSTEM_CLEANER_H
#define SYSTEM_CLEANER_H

#include <Windows.h>
#include <iostream>
#include <fstream>
#include "utils.h"
class SystemCleaner {
public:
	bool isJunkFile(const std::filesystem::path& filePath);
	void cleanFiles(const std::string& path);

private:
	UtilsClass utils;
};

#endif