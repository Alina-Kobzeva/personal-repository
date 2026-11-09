#pragma once
#include "GameField.h"
#include "PlacedRobot.h"
#include <memory>

class Factory {
private:
    int x, y;
    //  Раз в сколько ходов создавать врага
    int spawnEnemyInterval;
    // Сколько ходов прошло с прошлого создания
    int turnsPassed;

public:
    Factory(int startX, int startY, int interval);
    int getX() const;
    int getY() const;

    void blockedCells(GameField& Field);
    bool readyToSpawn();
    std::unique_ptr<PlacedRobot> newEnemy(int enemyNum);
};