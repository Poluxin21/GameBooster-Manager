#ifndef MEMORY_MANAGER_H 
#define MEMORY_MANAGER_H

#include <windows.h>
#include <psapi.h>
#include <vector>
#include <string>
#include <utility>
#include "utils.h"

using namespace std;

class MemoryManager
{
public:

	UtilsClass u;

	pair<int, int> memory_usage();

	SIZE_T memory_per_process(int processId);

	void suspend_process(int processId);

private:
	SIZE_T TotalMemoryUsage;
	PROCESS_MEMORY_COUNTERS pmc;
	HANDLE hProcess;
	int freeRam;
	int total;
};
#endif