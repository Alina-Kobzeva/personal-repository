#include "GameController.h"
#include "Constants.h"
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include "Teleport.h"
#include "Treatment.h"
#include "AttackOnTheSquare.h"
#include "FarAttack.h"
#include "SlowdownStatus.h"
#include "OverloadStatus.h"
#include "BurnStatus.h"
#include "ShieldStatus.h"

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
      spawnedCount(0), lastPlayerRank(1)
{
    enemies.push_back(std::make_unique<PlacedRobot>(realEnemy1, 7, 3));
    enemies.push_back(std::make_unique<PlacedRobot>(realEnemy2, 9, 8));

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

    if (std::rand() % 2 == 0) {
        enemies[0]->getRobot().getAbilities().push_back(std::make_unique<Treatment>());
    } else {
        enemies[0]->getRobot().getAbilities().push_back(std::make_unique<Teleport>());
    }

    if (std::rand() % 2 == 0) {
        enemies[1]->getRobot().getAbilities().push_back(std::make_unique<Treatment>());
    } else {
        enemies[1]->getRobot().getAbilities().push_back(std::make_unique<Teleport>());
    }

    // player.getRobot().addStatus(std::make_unique<ShieldStatus>(25));

    openFogOfWar();

    for (int y = 0; y < field.getHeight(); ++y) {
        for (int x = 0; x < field.getWidth(); ++x) {
            if (x == player.getX() && y == player.getY()) continue;

            Cell& cell = field.getCell(x, y);
            if (!cell.getIsWalkable()) continue;

            int roll = std::rand() % 100;
            int countToGen = 0;

            if (roll < 5) { 
                countToGen = 2;
            } else if (roll < 20) { 
                countToGen = 1;
            }
            for (int i = 0; i < countToGen; ++i) {
                int statusType = std::rand() % 4;
                if (statusType == 0) {
                    cell.addStatus(std::make_unique<SlowdownStatus>(Constants::SLOWDOWN_TURNS));
                } else if (statusType == 1) {
                    cell.addStatus(std::make_unique<BurnStatus>(Constants::BURN_TURNS, Constants::BURN_DAMAGE));
                } else if (statusType == 2) {
                    cell.addStatus(std::make_unique<OverloadStatus>());
                } else if (statusType == 3) {
                    cell.addStatus(std::make_unique<ShieldStatus>(Constants::SHIELD_POWER));
                }
            }
        }
    }
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
    for (auto& status : player.getRobot().getActiveStatuses()) {
        if (!status->canMove()) {
            std::cout << "Робот обездвижен статусом \"" << status->getName() << "!\nШаг невозможен.\n";
            player.getRobot().spendMovePoints(player.getRobot().getCurrentMovePoints());
            return;
        }
    }

    int nextX = player.getX() + stepX;
    int nextY = player.getY() + stepY;
        
    if (nextX >= field.getWidth() || nextX < 0 || nextY >= field.getHeight() || nextY < 0) return;

    if (!field.getCell(nextX, nextY).getIsWalkable()) return;

    int cost = field.getCell(nextX, nextY).getMovementCost();
    if (player.getRobot().getCurrentMovePoints() < cost) return;
    if (player.getRobot().getEnergy() < cost) return;

    bool hitEnemy = false;
    for (auto& enemy : enemies) {
        if (enemy->getX() == nextX && enemy->getY() == nextY && enemy->getRobot().getHealth() > 0) {
            player.getRobot().interact(enemy->getRobot());
            player.getRobot().spendEnergy(Constants::ENERGY_COMBAT_COST);
            checkAndProcessPlayerRankUp();
            hitEnemy = true;
            break;
        }
    }

    if (hitEnemy) return;

    player.setX(nextX);
    player.setY(nextY);

    player.getRobot().spendEnergy(cost);
    player.getRobot().spendMovePoints(cost);

    int currentCost = field.getCell(nextX, nextY).getMovementCost();

    Cell& steppedCell = field.getCell(nextX, nextY);
    if (steppedCell.hasStatuses()) {
        std::cout << "Робот наступил на аномальную клетку! Активированы эффекты:\n";
        for (auto& status : steppedCell.getCellStatuses()) {
            if (status->getName().find("Замедление") != std::string::npos) {
                player.getRobot().addStatus(std::make_unique<SlowdownStatus>(Constants::SLOWDOWN_TURNS));
            }
            else if (status->getName().find("Горение") != std::string::npos) {
                player.getRobot().addStatus(std::make_unique<BurnStatus>(Constants::BURN_TURNS, Constants::BURN_DAMAGE));
            }
            else if (status->getName().find("Перегрузка") != std::string::npos) {
                player.getRobot().addStatus(std::make_unique<OverloadStatus>());
            }
            else if (status->getName().find("Щит") != std::string::npos) {
                player.getRobot().addStatus(std::make_unique<ShieldStatus>(Constants::SHIELD_POWER));
            }

            std::cout << "Наложен статус: " << status->getName() << "\n";
        }

        steppedCell.removeOneRandomStatus();
    }
    openFogOfWar();
}

// Движение врага по направлению к игроку
void GameController::moveEnemies() {
    int pX = player.getX();
    int pY = player.getY();

    for (auto& enemy : enemies) {
        if (enemy->getRobot().getHealth() <= 0) continue;

        int enemySpeed = enemy->getRobot().getSpeed();
        for (int step = 0; step <enemySpeed; ++step) {
            int movX = 0;
            if (pX > enemy->getX()) movX = 1;
            else if (pX <  enemy->getX()) movX = -1;

            int movY = 0;
            if (pY > enemy->getY()) movY = 1;
            else if (pY < enemy->getY()) movY = -1;

            int newX = enemy->getX() + movX;
            int newY = enemy->getY() + movY;

            if (movX == 0 && movY == 0) break;

            if (newX < field.getWidth() && newX >= 0 && newY < field.getHeight() && newY >= 0) {
                if (field.getCell(newX, newY).getIsWalkable()) {
                    if (newX == pX && newY == pY) {
                        enemy->getRobot().interact(player.getRobot());
                        break;
                    }
                    else {
                        int cost = field.getCell(newX, newY).getMovementCost();
                        enemy->setX(newX);
                        enemy->setY(newY);
                    }
                } else {
                    break;
                }
            }
        }
    }
}

void GameController::selectStartingAbility() {
    std::cout << "\nВыберите стартовую способность:\n";
    std::cout << "[1] - Телепортация\n";
    std::cout << "[2] - Лечение\n";
    std::cout << "[3] - Удар по площади\n";
    std::cout << "[4] - Дальняя атака\n";
    std::cout << "Ваш выбор: ";

    int choice;
    std::cin >> choice;

    auto& pAbilities = player.getRobot().getAbilities();
    if (choice == 1) pAbilities.push_back(std::make_unique<Teleport>());
    else if (choice == 2) pAbilities.push_back(std::make_unique<Treatment>());
    else if (choice == 3) pAbilities.push_back(std::make_unique<AttackOnTheSquare>());
    else if (choice == 4) pAbilities.push_back(std::make_unique<FarAttack>());
    else {
        std::cout << "Неверный ввод. Автоматически выбрано Лечение.\n";
        pAbilities.push_back(std::make_unique<Treatment>());
    }
}

void GameController::checkAndProcessPlayerRankUp() {
    if (player.getRobot().getRank() > lastPlayerRank) {
        lastPlayerRank = player.getRobot().getRank();
        handlePlayerRankUp();
    }
}


void GameController::handlePlayerRankUp() {
    std::cout << "\nРАНГ ПОВЫШЕН! Выберите награду за уровень:\n";
    
    auto& pAbilities = player.getRobot().getAbilities();
    
    // Текущие способности для улучшения
    std::cout << "Улучшить имеющиеся способности:\n";
    for (size_t i = 0; i < pAbilities.size(); ++i) {
        std::cout << "[" << i + 1 << "] - Улучшить " << pAbilities[i]->getName() << "\n";
    }

    // Каких способностей еще нет, но можем изучить
    bool hasTeleport = false, hasHeal = false, hasAoE = false, hasRanged = false;
    for (const auto& ab : pAbilities) {
        if (ab->getName().find("Телепорт") != std::string::npos) hasTeleport = true;
        if (ab->getName().find("Лечение") != std::string::npos) hasHeal = true;
        if (ab->getName().find("Удар") != std::string::npos) hasAoE = true;
        if (ab->getName().find("Дальняя") != std::string::npos) hasRanged = true;
    }

    std::vector<std::string> lockedAbilities;
    if (!hasTeleport) lockedAbilities.push_back("Teleport");
    if (!hasHeal) lockedAbilities.push_back("Treatment");
    if (!hasAoE) lockedAbilities.push_back("AttackOnTheSquare");
    if (!hasRanged) lockedAbilities.push_back("FarAttack");

    int optionIndex = pAbilities.size() + 1;
    std::cout << "Изучить новые способности:\n";
    for (size_t i = 0; i < lockedAbilities.size(); ++i) {
        // Логичные названия для меню выбора
        std::string rusName = lockedAbilities[i];
        if (rusName == "Teleport") rusName = "Телепортация";
        if (rusName == "Treatment") rusName = "Лечение";
        if (rusName == "AttackOnTheSquare") rusName = "Удар по площади";
        if (rusName == "FarAttack") rusName = "Дальняя атака";
        
        std::cout << "[" << optionIndex + i << "] - Изучить " << rusName << "\n";
    }

    std::cout << "Ваш выбор: ";
    int choice;
    std::cin >> choice;

    if (choice >= 1 && choice <= static_cast<int>(pAbilities.size())) {
        pAbilities[choice - 1]->upgrade();
    } 
    else if (choice >= optionIndex && choice < optionIndex + static_cast<int>(lockedAbilities.size())) {
        std::string toLearn = lockedAbilities[choice - optionIndex];
        if (toLearn == "Teleport") pAbilities.push_back(std::make_unique<Teleport>());
        if (toLearn == "Treatment") pAbilities.push_back(std::make_unique<Treatment>());
        if (toLearn == "AttackOnTheSquare") pAbilities.push_back(std::make_unique<AttackOnTheSquare>());
        if (toLearn == "FarAttack") pAbilities.push_back(std::make_unique<FarAttack>());
        std::cout << "Новая способность успешно изучена!\n";
    } else {
        std::cout << "Неверный выбор, очки прокачки потеряны.\n";
    }
}


void GameController::usePlayerAbility(int idx) {
    auto& robot = player.getRobot();
    for (auto& status : robot.getActiveStatuses()) {
        if (!status->canUseAbilities()) {
            std::cout << "\nНаложена перегрузка! Способности заблокированы.\n";
            status->onAbilityAttempt();
            robot.tickStatuses(); 
            return;
        }
    }

    auto& ability = player.getRobot().getAbilities()[idx];
    int energyCost = ability->getEnergyCost();

    if (player.getRobot().getEnergy() < energyCost) {
        std::cout << "Недостаточно энергии для применения способности.\n";
        return;
    }

    player.getRobot().spendEnergy(energyCost);
    ability->execute(player, field, enemies);
    checkAndProcessPlayerRankUp();
}

void GameController::run() {
    char input = ' ';

    selectStartingAbility();

    while (player.getRobot().getHealth() > 0 && input != 'q' && input != 'Q') {
        player.getRobot().resetMovePoints();
        for (auto& enemy : enemies) {
            enemy->getRobot().resetMovePoints();
        }
        while (player.getRobot().getCurrentMovePoints() > 0 &&
               player.getRobot().getHealth() > 0 &&
               player.getRobot().getEnergy() > 0) {
            drawing.mDrawing(field, player, enemies, factory);
        

            bool anyEnemyAlive = false;
            for (const auto& enemy : enemies) {
                if (enemy->getRobot().getHealth() > 0) {
                    anyEnemyAlive = true;
                }
            }
            if (!anyEnemyAlive) {
                std::cout << "Победа! Все враги повержены!!!\n";
                return;
            }

        // Ввод/управление
            std::cout << "\nВаш ход. Осталось очков хода: "
                      << player.getRobot().getCurrentMovePoints() << "\n";
            std::cout << "Управление роботом:\nW (вверх)    S (вниз)\nA (влево)    D (вправо)\n";

        // Вывод способностей
            auto& playerAbilities = player.getRobot().getAbilities();
            std::cout << "Способности:\n";
            for (size_t i = 0; i < playerAbilities.size(); ++i) {
                std::cout << "[" << i + 1 << "] - " << playerAbilities[i]->getName()
                         << " (Энергия: " << playerAbilities[i]->getEnergyCost() << ")\n";
            }
            // std::cout << "\nЧтобы сменить режим подсветки нажмите 'V'\n";
            std::cout << "\nЧтобы завершить ходы досрочно нажмите 'E'\nЧтобы выйти из игры нажмите 'Q'\n";
            std::cout << "Ваш ход: ";
            std::cin >> input;

            if (input == 'q' || input == 'Q') return;

            if (input == 'w' || input == 'W') movePlayer(0, -1);
            else if (input == 's' || input == 'S') movePlayer(0, 1);
            else if (input == 'a' || input == 'A') movePlayer(-1, 0);
            else if (input == 'd' || input == 'D') movePlayer(1, 0);
            else if (input == 'v' || input == 'V') drawing.switchMode();

            else if (input >= '1' && input <= '9') {
                int abilityIdx = input - '1';
                if (abilityIdx < playerAbilities.size()) {
                    usePlayerAbility(abilityIdx);
                } else {
                    std::cout << "Неверный номер способности!\n";
                }
            }

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

        player.getRobot().tickStatuses();
        for (auto& enemy : enemies) {
            if (enemy->getRobot().getHealth() > 0) {
                enemy->getRobot().tickStatuses();
            }
        }

        player.getRobot().restoreEnergy(Constants::ENERGY_REGEN_PLAYER);
        for (auto& enemy : enemies) {
            enemy->getRobot().restoreEnergy(Constants::ENERGY_REGEN_ENEMY);
        }
    }

    if (player.getRobot().getHealth() <= 0) {
        std::cout << "Поражение! Попробуйте ещё раз.\n";
    }
}