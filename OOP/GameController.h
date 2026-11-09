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
    std::vector<std::unique_ptr<PlacedRobot>> enemies;
    int spawnedCount;

    void openFogOfWar();
    void movePlayer(int moveX, int moveY);
    void moveEnemies();

    void selectStartingAbility();
    void usePlayerAbility(int idx);
    void handlePlayerRankUp();
    int lastPlayerRank;
    void checkAndProcessPlayerRankUp();
public:
    GameController();
    void run();
};