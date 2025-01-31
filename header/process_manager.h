#ifndef PROCESSES_MANAGER_H 
#define PROCESSES_MANAGER_H
#define _NO_WINDOWS_BYTE

#include <windows.h>
#include <psapi.h>
#include <vector>
#include <string>
#include <utility>
#include <tlhelp32.h>
#include "utils.h"

class ProcessesManager
{
public:

	UtilsClass u;

	std::pair<unsigned int, std::vector<int>> num_process();

	std::string getProcessName(int pid);

	std::string getProcessfilter(std::string processName, int pid);

	HANDLE getProcessHandle(int pid);

	int definePriority(HANDLE hProcess);

	bool StopService(const char* serviceName);

	bool StartServiceProcess(const char* serviceName);

	void SetCPUAffinity(HANDLE hProcess, DWORD_PTR mask);


private:
	std::string processName;
	std::vector<int> numProcessid;
	DWORD processes[1024];
	DWORD cbNedeed;
	unsigned int i;
	HANDLE hProcessName;
	HANDLE hSnapshot;
	BOOL hResult;
	PROCESSENTRY32 pe;
};
#endif