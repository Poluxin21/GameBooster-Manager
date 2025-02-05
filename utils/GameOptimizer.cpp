#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "../header/GameOptimizer.h"
#include "../libs/nlohmann/json.hpp"

using json = nlohmann::json;

std::optional<std::vector<std::string>> GameOptimizer::loadGameConfig()
{
    std::ifstream file("games.json");
    std::vector<std::string> processesToMonitor;

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
        std::cerr << "Não foi possível abrir o arquivo de configuração!" << std::endl;
    }

    return processesToMonitor;
}

bool GameOptimizer::configureGameProcesses()
{
    /*utils.GetHighlevel();*/
    

    auto processList = processes.num_process();

    for (const int& pid : processList.second) {
        std::string processName = processes.getProcessName(pid);

        if (processName.empty())
        {
            utils.Logger("Não foi possivel obter o nome do processo");
            continue;
        }

        HANDLE processHandle = processes.getProcessHandle(pid);

        for (const std::string& filter : gameProcesses) {
            std::string filteresProcessPath = processes.getProcessfilter(filter, pid);


            if (!filteresProcessPath.empty()) {
            
                processes.definePriority(processHandle);
                processes.SetCPUAffinity(processHandle, DEFAULT_CPU_MASK);

                return true;
            }
        }
    }
    return false;
}

bool GameOptimizer::stopWindowsServices() {

    std::vector<std::string> services = {
        // Windows Update related
        "wuauserv", "UsoSvc", "bits",

        // System Services
        "SysMain", "DiagTrack", "dmwappushservice", "MapsBroker", "lfsvc",

        // Microsoft Services
        "OneDrive", "SearchIndexer", "MessagingService", "PushToInstall",

        // Print Services
        "spooler", "PrintNotify",

        // Remote Access/Sharing
        "RemoteRegistry", "RemoteAccess", "SharedAccess",

        // Optional Features
        "XblAuthManager", "XblGameSave", "XboxNetApiSvc", "WMPNetworkSvc",

        // Misc Services
        "WSearch", "iphlpsvc", "WbioSrvc", "FontCache",
        "TabletInputService", "AdobeARMservice",
        "GoogleChromeElevationService", "gupdate", "Steam Client Service"
    };

    bool success = true;
    std::vector<std::string> failedServices;

    for (const std::string& service : services) {
        if (!processes.StopService(service.c_str())) {
            failedServices.push_back(service);
            success = false;
        }
    }

    if (!success) {
        utils.Logger("Aviso: Serviços não podem ser parados");
        for (const std::string& service : failedServices) {
            utils.Logger("Erro service: " + service);
        }
    }

    return success;
}
