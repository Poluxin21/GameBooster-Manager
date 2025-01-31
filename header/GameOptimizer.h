#ifndef GAMER_OPTIMIZER_H
#define GAMER_OPTIMIZER_H
#include "process_manager.h"
#include "memory_manager.h"
#include "utils.h"
#include <optional>

class GameOptimizer {
public:
	std::optional<std::vector<std::string>> loadGameConfig();
	bool configureGameProcesses();
	bool stopWindowsServices();

    bool initialize() {
        auto loadedProcesses = loadGameConfig();
        if (!loadedProcesses) {
            return false;
        }
        gameProcesses = loadedProcesses.value();
        return !gameProcesses.empty();
    }

    bool optimize() {
        if (!configureGameProcesses()) {
            utils.Logger("Aviso: Nenhum processo de jogo foi configurado");
            return false;
        }

        if (!stopWindowsServices()) {
            utils.Logger("Aviso: Alguns serviços não puderam ser parados");
            return false;
        }

        utils.Logger("Otimização concluída com sucesso");
        return true;
    }


private: 
	ProcessesManager processes;
	MemoryManager memoryManager;
	UtilsClass utils;
	std::vector<std::string> gameProcesses;

	static constexpr DWORD_PTR DEFAULT_CPU_MASK = 0xF;
	static const std::string CONFIG_FILE_PATH;
};

#endif