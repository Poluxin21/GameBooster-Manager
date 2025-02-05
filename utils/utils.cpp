#include "../header/utils.h"
#include "../header/memory_manager.h"
#include <chrono>
#include <iostream>
#include <fstream>
#include <filesystem>

std::string UtilsClass::ExtractNameByPath(const std::string& path)
{
    return std::filesystem::path(path).filename().string();
}

std::string UtilsClass::GetTimeCurrent(const std::string& format)
{
    std::time_t now = std::time(nullptr);
    std::tm tstruct = {};
    std::stringstream ss;

#ifdef _WIN32
    localtime_s(&tstruct, &now);
#else
    localtime_r(&now, &tstruct);
#endif

    ss << std::put_time(&tstruct,
        (format == "now") ? "%Y-%m-%d %X" :
        (format == "date") ? "%Y-%m-%d" :
        "%Y-%m-%d %X");

    return ss.str();
}

void UtilsClass::Logger(const std::string& logMsg)
{
    try {
        std::filesystem::path logDir = "C:/turatlas";
        std::filesystem::create_directories(logDir);

        std::string logFileName = "log_" + GetTimeCurrent("date") + ".txt";
        std::filesystem::path logPath = logDir / logFileName;

        std::ofstream ofs(logPath, std::ios_base::out | std::ios_base::app);
        if (!ofs) {
            std::cerr << "Failed to open log file: " << logPath << std::endl;
            return;
        }

        std::string now = GetTimeCurrent("now");
        ofs << now << '\t' << logMsg << '\n';
        ofs.close();
    }
    catch (const std::exception& e) {
        std::cerr << "Logger error: " << e.what() << std::endl;
    }
}

//void UtilsClass::GetHighlevel()
//{
//    SHELLEXECUTEINFO sei = { sizeof(sei) };
//    sei.lpVerb = L"runas";
//    sei.lpFile = L"TurAtlas.exe";
//    sei.nShow = SW_SHOWNORMAL;
//
//    if (!ShellExecuteEx(&sei)) {
//        std::cerr << "Erro ao elevar permissões!" << std::endl;
//        Logger("Erro ao elevar permissões!");
//    }
//}