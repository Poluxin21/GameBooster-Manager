#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <dwmapi.h>
#include "../header/process_manager.h"

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
	std::wstring wServiceName;
	int len = MultiByteToWideChar(CP_UTF8, 0, serviceName, -1, NULL, 0);
	if (len > 0) {
		wServiceName.resize(len);
		MultiByteToWideChar(CP_UTF8, 0, serviceName, -1, &wServiceName[0], len);
	}
	else {
		std::cerr << "Erro na conversão do nome do serviço" << std::endl;
		return false;
	}

	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hSCManager) {
		DWORD error = GetLastError();
		std::cerr << "Falha ao abrir o gerenciador de serviços. Erro: " << error << std::endl;
		return false;
	}

	SC_HANDLE hService = OpenService(hSCManager, wServiceName.c_str(), SERVICE_STOP | SERVICE_QUERY_STATUS);
	if (!hService) {
		DWORD error = GetLastError();
		std::cerr << "Falha ao abrir o serviço: " << serviceName << ". Erro: " << error << std::endl;
		CloseServiceHandle(hSCManager);
		return false;
	}

	SERVICE_STATUS_PROCESS ssp;
	DWORD bytesNeeded;
	if (!QueryServiceStatusEx(hService, SC_STATUS_PROCESS_INFO,
		(LPBYTE)&ssp, sizeof(SERVICE_STATUS_PROCESS), &bytesNeeded)) {
		DWORD error = GetLastError();
		std::cerr << "Falha ao consultar status do serviço. Erro: " << error << std::endl;
		CloseServiceHandle(hService);
		CloseServiceHandle(hSCManager);
		return false;
	}

	if (ssp.dwCurrentState == SERVICE_STOPPED) {
		std::cout << "Serviço já está parado: " << serviceName << std::endl;
		CloseServiceHandle(hService);
		CloseServiceHandle(hSCManager);
		return true;
	}

	SERVICE_STATUS serviceStatus;
	if (!ControlService(hService, SERVICE_CONTROL_STOP, &serviceStatus)) {
		DWORD error = GetLastError();
		std::cerr << "Falha ao parar o serviço: " << serviceName << ". Erro: " << error << std::endl;
		CloseServiceHandle(hService);
		CloseServiceHandle(hSCManager);
		return false;
	}

	DWORD startTime = GetTickCount();
	DWORD timeout = 30000;

	while (serviceStatus.dwCurrentState != SERVICE_STOPPED) {
		Sleep(250);

		if (!QueryServiceStatusEx(hService, SC_STATUS_PROCESS_INFO,
			(LPBYTE)&ssp, sizeof(SERVICE_STATUS_PROCESS), &bytesNeeded)) {
			break;
		}

		if (GetTickCount() - startTime > timeout) {
			std::cerr << "Timeout ao aguardar o serviço parar: " << serviceName << std::endl;
			CloseServiceHandle(hService);
			CloseServiceHandle(hSCManager);
			return false;
		}
	}

	std::cout << "Serviço parado com sucesso: " << serviceName << std::endl;
	CloseServiceHandle(hService);
	CloseServiceHandle(hSCManager);
	return true;
}


bool ProcessesManager::StartServiceProcess(const char* serviceName) {
	std::wstring wServiceName;
	int len = MultiByteToWideChar(CP_UTF8, 0, serviceName, -1, NULL, 0);
	if (len > 0) {
		wServiceName.resize(len);
		MultiByteToWideChar(CP_UTF8, 0, serviceName, -1, &wServiceName[0], len);
	}
	else {
		std::cerr << "Erro na conversão do nome do serviço" << std::endl;
		return false;
	}

	SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hSCManager) {
		DWORD error = GetLastError();
		std::cerr << "Falha ao abrir o gerenciador de serviços. Erro: " << error << std::endl;
		return false;
	}

	SC_HANDLE hService = OpenService(hSCManager, wServiceName.c_str(),
		SERVICE_START | SERVICE_QUERY_STATUS);
	if (!hService) {
		DWORD error = GetLastError();
		std::cerr << "Falha ao abrir o serviço: " << serviceName << ". Erro: " << error << std::endl;
		CloseServiceHandle(hSCManager);
		return false;
	}

	SERVICE_STATUS_PROCESS ssp;
	DWORD bytesNeeded;
	if (!QueryServiceStatusEx(hService, SC_STATUS_PROCESS_INFO,
		(LPBYTE)&ssp, sizeof(SERVICE_STATUS_PROCESS), &bytesNeeded)) {
		DWORD error = GetLastError();
		std::cerr << "Falha ao consultar status do serviço. Erro: " << error << std::endl;
		CloseServiceHandle(hService);
		CloseServiceHandle(hSCManager);
		return false;
	}

	if (ssp.dwCurrentState == SERVICE_RUNNING) {
		std::cout << "Serviço já está em execução: " << serviceName << std::endl;
		CloseServiceHandle(hService);
		CloseServiceHandle(hSCManager);
		return true;
	}


	if (ssp.dwCurrentState == SERVICE_START_PENDING) {
		std::cout << "Serviço já está iniciando: " << serviceName << std::endl;
	}

	else if (ssp.dwCurrentState == SERVICE_STOPPED) {
		if (!StartService(hService, 0, NULL)) {
			DWORD error = GetLastError();
			std::cerr << "Falha ao iniciar o serviço: " << serviceName << ". Erro: " << error << std::endl;
			CloseServiceHandle(hService);
			CloseServiceHandle(hSCManager);
			return false;
		}
	}

	DWORD startTime = GetTickCount();
	DWORD timeout = 30000;

	while (ssp.dwCurrentState != SERVICE_RUNNING) {
		Sleep(250);

		if (!QueryServiceStatusEx(hService, SC_STATUS_PROCESS_INFO,
			(LPBYTE)&ssp, sizeof(SERVICE_STATUS_PROCESS), &bytesNeeded)) {
			DWORD error = GetLastError();
			std::cerr << "Falha ao consultar status do serviço durante inicialização. Erro: " << error << std::endl;
			break;
		}

		if (GetTickCount() - startTime > timeout) {
			std::cerr << "Timeout ao aguardar o serviço iniciar: " << serviceName << std::endl;
			CloseServiceHandle(hService);
			CloseServiceHandle(hSCManager);
			return false;
		}
	}

	std::cout << "Serviço iniciado com sucesso: " << serviceName << std::endl;
	CloseServiceHandle(hService);
	CloseServiceHandle(hSCManager);
	return true;
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