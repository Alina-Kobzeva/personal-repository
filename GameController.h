#pragma once
#include "PlacedRobot.h"
#include "MapDrawing.h"
#include "GameField.h"
#include "Factory.h"

class GameController {
private:
    PlacedRobot player;
    MapDrawing drawing;
    GameField field;
    Factory factory;
    std::vector<PlacedRobot> enemies;
    int spawnedCount;

    void openFogOfWar();
    void movePlayer(int moveX, int moveY);
    void moveEnemies();

public:
    GameController();
    void run();
};