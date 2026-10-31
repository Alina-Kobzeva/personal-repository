#include "GameController.h"
#include <ctime>
#include <cstdlib>

int main() {
    // std::srand(static_cast<unsigned int>(std::time(nullptr)));
    GameController game;
    game.run();

    return 0;
}