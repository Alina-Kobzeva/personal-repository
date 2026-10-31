#include "Factory.h"
#include "Constants.h"

Factory::Factory(int startX, int startY, int interval)
    : x(startX), y(startY), spawnEnemyInterval(interval), turnsPassed(0) {}

int Factory::getX() const { return x; }
int Factory::getY() const { return y; }

void Factory::blockedCells(GameField& field) {
    field.getCell(x, y).setIsWalkable(false);
    field.getCell(x + 1, y).setIsWalkable(false);
    field.getCell(x, y + 1).setIsWalkable(false);
    field.getCell(x + 1, y + 1).setIsWalkable(false);
}

bool Factory::readyToSpawn() {
    turnsPassed++;
    if (turnsPassed >= spawnEnemyInterval) {
        turnsPassed = 0;
        return true;
    }
    return false;
}

PlacedRobot Factory::newEnemy(int enemyNum) {
    Robot* newRobot = new Robot("Боевой робот-враг-" + std::to_string(enemyNum),
    Constants::ENEMY_MAX_HP, Constants::NEW_ENEMY_MAX_ENERGY,
    Constants::NEW_ENEMY_ATTACK, Constants::ENEMY_SPEED,
    Constants::ENEMY_VISION, true);
    return PlacedRobot(*newRobot, x, y + Constants::FACTORY_SIZE);
}