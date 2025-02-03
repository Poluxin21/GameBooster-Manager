#define _NO_WINDOWS_BYTE
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <windows.h>
#include "libs/nlohmann/json.hpp"
#include "header/GameOptimizer.h"


int main() {
    GameOptimizer gameOptimizer;
    gameOptimizer.initialize();
    gameOptimizer.optimize();
    return 0;
}
