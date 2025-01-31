#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include <windows.h>
#include "header/memory_manager.h"
#include "header/process_manager.h"
#include "header/utils.h"

using json = nlohmann::json;

using namespace std;

vector<string> loadGameConfig() {
    ifstream file("games.json");
    vector<string> processesToMonitor;

    if (file.is_open()) {
        json gamesData;
        file >> gamesData;

        for (const auto& game : gamesData["games"]) {
            for (const auto& process : game["processes"]) {
                processesToMonitor.push_back(process);
            }
        }
    }
    else {
        cerr << "Não foi possível abrir o arquivo de configuração!" << endl;
    }

    return processesToMonitor;
}

int configureProcess(vector<string>& gameProcesses) {
    ProcessesManager p;
    MemoryManager m;
    UtilsClass ut;

    string proc;
    HANDLE handleProc;
    DWORD_PTR mask = 0xF;

    auto numProcess = p.num_process();

    for (const int& pid : numProcess.second) {
        string name = p.getProcessName(pid);
        if (!name.empty()) {
            handleProc = p.getProcessHandle(pid);
            p.SetCPUAffinity(handleProc, mask);

            for (const string& filter : gameProcesses) {
                proc = p.getProcessfilter(filter, pid);

                if (!proc.empty()) {
                    string pathProc = ut.ExtractNameByPath(proc);
                    cout << "Nome: " << pathProc << " Pid: " << pid << endl;
                    //p.definePriority(handleProc);
                }
            }
        }
    }

    return 1;
}

int StopWinServices() {
    ProcessesManager p;
    std::vector<const char*> services = {
        // Windows Update related
        "wuauserv",
        "UsoSvc",
        "bits",

        // System Services
        "SysMain",
        "DiagTrack",
        "dmwappushservice",
        "MapsBroker",
        "lfsvc",

        // Microsoft Services
        "OneDrive",
        "SearchIndexer",
        "MessagingService",
        "PushToInstall",

        // Print Services
        "spooler",
        "PrintNotify",

        // Remote Access/Sharing
        "RemoteRegistry",
        "RemoteAccess",
        "SharedAccess",

        // Optional Features
        "XblAuthManager",
        "XblGameSave",
        "XboxNetApiSvc",
        "WMPNetworkSvc",

        // Misc Services
        "WSearch",
        "iphlpsvc",
        "WbioSrvc",
        "FontCache",
        "TabletInputService",
        "AdobeARMservice",
        "GoogleChromeElevationService",
        "gupdate",
        "Steam Client Service"
    };

    bool allStopped = true;
    std::vector<const char*> failedServices;

    for (const char* service : services) {
        if (!p.StopService(service)) {
            allStopped = false;
            failedServices.push_back(service);
        }
    }
    return allStopped ? 0 : 1;
}

int main() {
    vector<string> gameProcesses = loadGameConfig();

    if (!gameProcesses.empty())
    {
        configureProcess(gameProcesses);
        int result = StopWinServices();
        if (result == 0) {
            std::cout << "Todos os serviços foram parados com sucesso." << std::endl;
        }
        else {
            std::cout << "Alguns serviços falharam ao serem parados." << std::endl;
        }
    }
    else
    {
        cout << "Não foi possivel encontrar nenhum jogo para otimizar";
        return 0;
    }
    

    return 0;
}
