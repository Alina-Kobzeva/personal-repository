#include "GameController.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <cmath>

static Robot realPlayer("Маленький робот", 100, 50, 20, 3, 1, false);
static Robot realEnemy1("Робот-враг-1", 40, 20, 15, 2, 1, true);
static Robot realEnemy2("Робот-враг-2", 40, 20, 15, 2, 1, true);

GameController::GameController()
    : player(realPlayer, 1, 1), drawing(), 
      field(16, 9), factory(7, 4, 5), spawnedCount(0)
{
    enemies.push_back(PlacedRobot(realEnemy1, 7, 3));
    enemies.push_back(PlacedRobot(realEnemy2, 9, 8));

    factory.blockedCells(field);

    field.getCell(3, 2).setIsWalkable(false);
    field.getCell(2, 3).setIsWalkable(false);
    field.getCell(5, 3).setIsWalkable(false);
    field.getCell(4, 3).setIsWalkable(false);


    field.getCell(3, 7).setMovementCost(2);
    field.getCell(4, 7).setMovementCost(2);
    field.getCell(5, 7).setMovementCost(2);
    field.getCell(4, 8).setMovementCost(2);

    field.getCell(12, 6).setMovementCost(2);
    field.getCell(13, 6).setMovementCost(2);
    field.getCell(13, 7).setMovementCost(2);

    openFogOfWar();
}

void GameController::openFogOfWar() {
    int pX = player.getX();
    int pY = player.getY();
    int radius = player.getRobot().getVisionRadius();

    for (int y = pY - radius; y <= pY + radius; ++y) {
        for (int x = pX - radius; x <= pX + radius; ++x) {
            if (x >= 0 && x < field.getWidth() && y >= 0 && y < field.getHeight()) {
                field.getCell(x, y).setIsDiscovered(true);
            }
        }
    }
}

void GameController::movePlayer(int movX, int movY) {
    int stepX = (movX > 0) ? 1 : ((movX < 0) ? -1 : 0);
    int stepY = (movY > 0) ? 1 : ((movY < 0) ? -1 : 0);

    int totalSteps = std::max(std::abs(movX), std::abs(movY));

    for (int i = 0; i < totalSteps; ++i) {
        int nextX = player.getX() + stepX;
        int nextY = player.getY() + stepY;
        
        if (nextX >= field.getWidth() || nextX < 0 || nextY >= field.getHeight() || nextY < 0) break;

        if (!field.getCell(nextX, nextY).getIsWalkable()) break;

        int cost = field.getCell(nextX, nextY).getMovementCost();
        if (player.getRobot().getCurrentMovePoints() < cost) break;

        if (player.getRobot().getEnergy() < cost) break;

        bool hitEnemy = false;
        for (auto& enemy : enemies) {
            if (enemy.getX() == nextX && enemy.getY() == nextY && enemy.getRobot().getHealth() > 0) {
                player.getRobot().interact(enemy.getRobot());
                player.getRobot().spendEnergy(2);
                hitEnemy = true;
                break;
            }
        }

        if (hitEnemy) break;

        player.setX(nextX);
        player.setY(nextY);

        player.getRobot().spendEnergy(cost);
        player.getRobot().spendMovePoints(cost);

        openFogOfWar();
    }
}

// Движение врага по направлению к игроку
void GameController::moveEnemies() {
    int pX = player.getX();
    int pY = player.getY();

    for (auto& enemy : enemies) {
        if (enemy.getRobot().getHealth() <= 0) continue;

        int enemySpeed = enemy.getRobot().getSpeed();
        for (int step = 0; step <enemySpeed; ++step) {
            int movX = 0;
            if (pX > enemy.getX()) movX = 1;
            else if (pX <  enemy.getX()) movX = -1;

            int movY = 0;
            if (pY > enemy.getY()) movY = 1;
            else if (pY < enemy.getY()) movY = -1;

            int newX = enemy.getX() + movX;
            int newY = enemy.getY() + movY;

            if (movX == 0 && movY == 0) break;

            if (newX < field.getWidth() && newX >= 0 && newY < field.getHeight() && newY >= 0) {
                if (field.getCell(newX, newY).getIsWalkable()) {
                    if (newX == pX && newY == pY) {
                        enemy.getRobot().interact(player.getRobot());
                        break;
                    }
                    else {
                        int cost = field.getCell(newX, newY).getMovementCost();
                        enemy.setX(newX);
                        enemy.setY(newY);
                    }
                } else {
                    break;
                }
            }
        }
    }
}

void GameController::run() {
    char input = ' ';

    while (player.getRobot().getHealth() > 0 && input != 'q' && input != 'Q') {
        player.getRobot().resetMovePoints();
        for (auto& enemy : enemies) {
            enemy.getRobot().resetMovePoints();
        }

        drawing.mDrawing(field, player, enemies, factory);

        bool anyEnemyAlive = false;
        for (const auto& enemy : enemies) {
            if (enemy.getRobot().getHealth() > 0) {
                anyEnemyAlive = true;
            }
        }
        if (!anyEnemyAlive) {
            std::cout << "Победа! Все враги повержены!!!";
            break;
        }

        // Ввод/управление
        std::cout << "Управление роботом:\nW (вверх)    S (вниз)\nA (влево)    D (вправо)\n";
        std::cout << "Чтобы выйти из игры нажмите 'Q'\n";
        std::cout << "Ваш ход: ";
        std::cin >> input;

        int currentSpeed = player.getRobot().getSpeed();

        if (input == 'w' || input == 'W') movePlayer(0, -currentSpeed);
        else if (input == 's' || input == 'S') movePlayer(0, currentSpeed);
        else if (input == 'a' || input == 'A') movePlayer(-currentSpeed, 0);
        else if (input == 'd' || input == 'D') movePlayer(currentSpeed, 0);
        else continue;
        moveEnemies();

        if (factory.readyToSpawn()) {
            spawnedCount++;
            enemies.push_back(factory.newEnemy(spawnedCount));
        }

        player.getRobot().restoreEnergy(0);
        for (auto& enemy : enemies) {
            enemy.getRobot().restoreEnergy(0);
        }
    }
    if (player.getRobot().getHealth() <= 0) {
        std::cout << "Поражение! Попробуйте ещё раз.\n";
    }
}