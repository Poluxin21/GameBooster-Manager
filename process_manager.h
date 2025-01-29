#ifndef PROCESSES_MANAGER_H 
#define PROCESSES_MANAGER_H


#include <windows.h>
#include <psapi.h>
#include <vector>
#include <string>
#include <utility>
#include <tlhelp32.h>
#include "utils.h"

using namespace std;

class ProcessesManager
{
public:

	UtilsClass u;

	pair<unsigned int, vector<int>> num_process();

	string getProcessName(int pid);

	string getProcessfilter(string processName, int pid);

	HANDLE getProcessHandle(int pid);

	int definePriority(HANDLE hProcess);

	bool StopService(const char* serviceName);

	bool StartServiceProcess(const char* serviceName);

	void SetCPUAffinity(HANDLE hProcess, DWORD_PTR mask);


private:
	string processName;
	vector<int> numProcessid;
	DWORD processes[1024];
	DWORD cbNedeed;
	unsigned int i;
	HANDLE hProcessName;
	HANDLE hSnapshot;
	BOOL hResult;
	PROCESSENTRY32 pe;
};
#endif