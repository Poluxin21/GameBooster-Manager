#include "../header/SystemCleaner.h"
#include <iostream>
#include <filesystem>
#include <cstdio>
#include <vector>
#include <fstream>

bool SystemCleaner::isJunkFile(const std::filesystem::path& filePath)
{
    std::vector<std::string> extensoesInuteis = { ".tmp", ".log", ".bak", ".dmp", ".cache" };
 
    std::string ext = filePath.extension().string();
    return std::find(extensoesInuteis.begin(), extensoesInuteis.end(), ext) != extensoesInuteis.end();
}

void SystemCleaner::cleanFiles(const std::string& path)
{
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (std::filesystem::is_regular_file(entry) && isJunkFile(entry.path())) {
            try {
                std::cout << "Removendo: " << entry.path() << std::endl;
                std::filesystem::remove(entry);
            }
            catch (const std::filesystem::filesystem_error& e) {
                std::cerr << "Erro ao remover " << entry.path() << ": " << e.what() << std::endl;
            }
        }
    }
}