#include "GameController.h"
#include "Constants.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <cmath>

static Robot realPlayer(Constants::PLAYER_NAME, Constants::PLAYER_MAX_HP, Constants::PLAYER_MAX_ENERGY,
        Constants::PLAYER_ATTACK, Constants::PLAYER_SPEED, Constants::PLAYER_VISION, false);

static Robot realEnemy1("Робот-враг-1", Constants::ENEMY_MAX_HP, Constants::ENEMY_MAX_ENERGY,
        Constants::ENEMY_ATTACK, Constants::ENEMY_SPEED, Constants::ENEMY_VISION, true);

static Robot realEnemy2("Робот-враг-2", Constants::ENEMY_MAX_HP, Constants::ENEMY_MAX_ENERGY,
        Constants::ENEMY_ATTACK, Constants::ENEMY_SPEED, Constants::ENEMY_VISION, true);


GameController::GameController()
    : player(realPlayer, 1, 1), drawing(), 
      field(Constants::DEFAULT_FIELD_WIDTH, Constants::DEFAULT_FIELD_HEIGHT), 
      factory(Constants::FACTORY_X, Constants::FACTORY_Y, Constants::FACTORY_SPAWN_INTERVAL), 
      spawnedCount(0)
{
    enemies.push_back(PlacedRobot(realEnemy1, 7, 3));
    enemies.push_back(PlacedRobot(realEnemy2, 9, 8));

    factory.blockedCells(field);

    field.getCell(3, 2).setIsWalkable(false);
    field.getCell(2, 3).setIsWalkable(false);
    field.getCell(5, 3).setIsWalkable(false);
    field.getCell(4, 3).setIsWalkable(false);


    field.getCell(3, 7).setMovementCost(Constants::COST_HEAVY_TERRAIN);
    field.getCell(4, 7).setMovementCost(Constants::COST_HEAVY_TERRAIN);
    field.getCell(5, 7).setMovementCost(Constants::COST_HEAVY_TERRAIN);
    field.getCell(4, 8).setMovementCost(Constants::COST_HEAVY_TERRAIN);

    field.getCell(12, 6).setMovementCost(Constants::COST_HEAVY_TERRAIN);
    field.getCell(13, 6).setMovementCost(Constants::COST_HEAVY_TERRAIN);
    field.getCell(13, 7).setMovementCost(Constants::COST_HEAVY_TERRAIN);

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

void GameController::movePlayer(int stepX, int stepY) {
    int nextX = player.getX() + stepX;
    int nextY = player.getY() + stepY;
        
    if (nextX >= field.getWidth() || nextX < 0 || nextY >= field.getHeight() || nextY < 0) return;

    if (!field.getCell(nextX, nextY).getIsWalkable()) return;

    int cost = field.getCell(nextX, nextY).getMovementCost();
    if (player.getRobot().getCurrentMovePoints() < cost) return;
    if (player.getRobot().getEnergy() < cost) return;

    bool hitEnemy = false;
    for (auto& enemy : enemies) {
        if (enemy.getX() == nextX && enemy.getY() == nextY && enemy.getRobot().getHealth() > 0) {
            player.getRobot().interact(enemy.getRobot());
            player.getRobot().spendEnergy(Constants::ENERGY_COMBAT_COST);
            hitEnemy = true;
            break;
        }
    }

    if (hitEnemy) return;

    player.setX(nextX);
    player.setY(nextY);

    player.getRobot().spendEnergy(cost);
    player.getRobot().spendMovePoints(cost);

    openFogOfWar();
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
        while (player.getRobot().getCurrentMovePoints() > 0 && player.getRobot().getHealth() > 0) {
            drawing.mDrawing(field, player, enemies, factory);
        

            bool anyEnemyAlive = false;
            for (const auto& enemy : enemies) {
                if (enemy.getRobot().getHealth() > 0) {
                    anyEnemyAlive = true;
                }
            }
            if (!anyEnemyAlive) {
                std::cout << "Победа! Все враги повержены!!!\n";
                return;
            }

        // Ввод/управление
            std::cout << "Ваш ход. Осталось очков хода: "
                      << player.getRobot().getCurrentMovePoints() << "\n";
            std::cout << "Управление роботом:\nW (вверх)    S (вниз)\nA (влево)    D (вправо)\n";
            std::cout << "Чтобы завершить ходы досрочно нажмите 'E'\nЧтобы выйти из игры нажмите 'Q'\n";
            std::cout << "Ваш ход: ";
            std::cin >> input;

            if (input == 'q' || input == 'Q') return;

            if (input == 'w' || input == 'W') movePlayer(0, -1);
            else if (input == 's' || input == 'S') movePlayer(0, 1);
            else if (input == 'a' || input == 'A') movePlayer(-1, 0);
            else if (input == 'd' || input == 'D') movePlayer(1, 0);
            else if (input == 'e' || input == 'E') {
                //  std::cout << "Вы досрочно завершили ход.\n";
                break;
            } else continue;
        }

        if (player.getRobot().getHealth() <= 0) break;

        moveEnemies();

        if (factory.readyToSpawn()) {
            spawnedCount++;
            enemies.push_back(factory.newEnemy(spawnedCount));
        }

        player.getRobot().restoreEnergy(Constants::ENERGY_REGEN_PLAYER);
        for (auto& enemy : enemies) {
            enemy.getRobot().restoreEnergy(Constants::ENERGY_REGEN_ENEMY);
        }
    }

    if (player.getRobot().getHealth() <= 0) {
        std::cout << "Поражение! Попробуйте ещё раз.\n";
    }
}