#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
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
        std::cerr << "N�o foi poss�vel abrir o arquivo de configura��o!" << std::endl;
    }

    return processesToMonitor;
}

bool GameOptimizer::configureGameProcesses()
{

    auto processList = processes.num_process();

    for (const int& pid : processList.second) {
        std::string processName = processes.getProcessName(pid);

        if (processName.empty())
        {
            utils.Logger("N�o foi possivel obter o nome do processo");
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
        utils.Logger("Aviso: Servi�os n�o podem ser parados");
        for (const std::string& service : failedServices) {
            utils.Logger("Erro service: " + service);
        }
    }

    return success;
}

bool GameOptimizer::cleanJunkFiles()
{
    char* username = nullptr;
    size_t len;

    if (_dupenv_s(&username, &len, "USERNAME") != 0 || username == nullptr) {
        std::cerr << "Erro ao obter nome de usu�rio." << std::endl;
        return false;
    }

    std::string pathBase = "C:\\Users\\" + std::string(username);
    free(username);

    std::string pathArray[] = {
        "C:\\Windows\\Temp\\",
        "\\AppData\\Local\\CrashDumps\\",
        "C:\\ProgramData\\Microsoft\\Windows\\WER\\",
        "\\AppData\\Local\\Temp\\"
    };

    bool foundFiles = false;

    for (size_t i = 0; i < (sizeof(pathArray) / sizeof(pathArray[0])); i++) {
        std::string pathName = pathArray[i];

        if (pathName.starts_with("C")) {
            systemCleaner.cleanFiles(pathName);
        }
        else {
            systemCleaner.cleanFiles(pathBase + pathName);
        }

        foundFiles = true;
    }

    return foundFiles;
}