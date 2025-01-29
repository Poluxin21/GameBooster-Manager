#include "process_manager.h"
#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <tlhelp32.h>
#include "utils.h"
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
using namespace std;

pair<unsigned int, vector<int>> ProcessesManager::num_process()
{
	if (EnumProcesses(processes, sizeof(processes), &cbNedeed))
	{
		unsigned int numProcesses = cbNedeed / sizeof(DWORD);

		vector<int> numProcessid;
		for (i = 0; i < numProcesses; i++)
		{
			numProcessid.push_back(static_cast<int>(processes[i]));
		}
		return make_pair(numProcesses, numProcessid);
	}
	else
	{
		u.Logger("Não foi possível encontrar nenhum processo");
		return make_pair(0, vector<int>());
	}
}

string ProcessesManager::getProcessName(int pid)
{

	HANDLE handle = OpenProcess(
		PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
		FALSE,
		(DWORD)pid
	);
	if (handle)
	{

		DWORD size = MAX_PATH;
		DWORD buffSize = 1024;
		CHAR buffer[1024];
		if (QueryFullProcessImageNameA(handle, 0, buffer, &buffSize))
		{
			processName = buffer;
		}
		else
		{
			DWORD error = GetLastError();
			u.Logger("Error QueryFullProcessImageNameA ");
		}
		CloseHandle(handle);
	}
	return processName;
}


string ProcessesManager::getProcessfilter(string filter, int pid)
{
	string ProcessName = getProcessName(pid);
	if (!ProcessName.empty())
	{
		if (ProcessName.find(filter) != string::npos)
		{
			return ProcessName;
		}
	}
	else
	{
		u.Logger("Pid não encontrado");
	}

	return "";

}

HANDLE ProcessesManager::getProcessHandle(int pid)
{
	HANDLE handle = OpenProcess(
		PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
		FALSE,
		(DWORD)pid
	);

	if (handle == NULL)
	{
		return NULL;
	}

	return handle;

}

int ProcessesManager::definePriority(HANDLE processName)
{
	SetPriorityClass(processName, REALTIME_PRIORITY_CLASS);
	return 0;
}

bool ProcessesManager::StopService(const char* serviceName)
{
	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hSCManager) {
		std::cerr << "Falha ao abrir o gerenciador de serviços." << std::endl;
		return false;
	}

	SC_HANDLE hService = OpenService(hSCManager, (LPCWSTR)serviceName, SERVICE_STOP | SERVICE_QUERY_STATUS);
	if (!hService) {
		std::cerr << "Falha ao abrir o serviço: " << serviceName << std::endl;
		CloseServiceHandle(hSCManager);
		return false;
	}

	SERVICE_STATUS serviceStatus = {};
	bool result = ControlService(hService, SERVICE_CONTROL_STOP, &serviceStatus);

	if (result) {
		std::cout << "Serviço parado: " << serviceName << std::endl;
	}
	else {
		std::cerr << "Falha ao parar o serviço: " << serviceName << std::endl;
	}

	CloseServiceHandle(hService);
	CloseServiceHandle(hSCManager);
	return result;
}

bool ProcessesManager::StartServiceProcess(const char* serviceName) {
	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hSCManager) {
		std::cerr << "Falha ao abrir o gerenciador de serviços." << std::endl;
		return false;
	}

	SC_HANDLE hService = OpenService(hSCManager, (LPCWSTR)serviceName, SERVICE_START);
	if (!hService) {
		std::cerr << "Falha ao abrir o serviço: " << serviceName << std::endl;
		CloseServiceHandle(hSCManager);
		return false;
	}

	if (StartService(hService, 0, NULL)) {
		std::cout << "Serviço iniciado: " << serviceName << std::endl;
		return true;
	}
	else {
		std::cerr << "Falha ao iniciar o serviço: " << serviceName << std::endl;
		return false;
	}

	CloseServiceHandle(hService);
	CloseServiceHandle(hSCManager);
}

void ProcessesManager::SetCPUAffinity(HANDLE hProcess, DWORD_PTR mask)
{
	if (SetProcessAffinityMask(hProcess, mask)) {
		std::cout << "Afinidade do processo definida com sucesso." << std::endl;
	}
	else {
		std::cerr << "Falha ao definir afinidade do processo." << std::endl;
	}
}