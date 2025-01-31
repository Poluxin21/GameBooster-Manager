#define _NO_WINDOWS_BYTE
#include <windows.h>
#include <psapi.h>
#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <TlHelp32.h>
#include "../header/memory_manager.h"

std::pair<int, int> MemoryManager::memory_usage()
{
    MEMORYSTATUSEX stats{};
    stats.dwLength = sizeof(stats);

    freeRam = stats.ullAvailPhys / 1024 / 1024;
    total = stats.ullTotalPhys / 1024 / 1024;


    if (GlobalMemoryStatusEx(&stats))
    {
        return std::make_pair(freeRam, total);
    }
    else
    {
        u.Logger("Não foi possivel encontrar informações sobre o Uso de memoria");
    }
}

void MemoryManager::suspend_process(int processId)
{
    HANDLE hThreadSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

    THREADENTRY32 threadEntry;
    threadEntry.dwSize = sizeof(THREADENTRY32);

    Thread32First(hThreadSnapshot, &threadEntry);

    do
    {
        if (threadEntry.th32OwnerProcessID == (DWORD)processId)
        {
            HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE,
                threadEntry.th32ThreadID);

            if (hThread != NULL)
            {
                SuspendThread(hThread);
                CloseHandle(hThread);
            }
            else
            {
                u.Logger("Não foi possivel encontrar o processo para suspender");
            }
        }
    } while (Thread32Next(hThreadSnapshot, &threadEntry));

    CloseHandle(hThreadSnapshot);
}

SIZE_T MemoryManager::memory_per_process(int processId)
{
    hProcess = OpenProcess(
        PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
        FALSE, (DWORD)processId
    );

    if (hProcess == NULL)
    {
        u.Logger("Não foi possivel verificar o processo pid: " + processId);
    }

    if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc)))
    {
        TotalMemoryUsage = pmc.WorkingSetSize + pmc.PagefileUsage;
        return TotalMemoryUsage / (1024 * 1024);
    }

    CloseHandle(hProcess);
    return 0;
}