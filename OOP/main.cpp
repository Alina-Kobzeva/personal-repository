#include "GameController.h"
#include <ctime>
#include <cstdlib>

// #ifdef _WIN32
// system("chcp 65001 > nul");
// #endif

int main() {
    // std::srand(static_cast<unsigned int>(std::time(nullptr)));
    GameController game;
    game.run();

    return 0;
}