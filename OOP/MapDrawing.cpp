#include "MapDrawing.h"
#include "Constants.h"
#include <cstdlib>
#include <iostream>
#include <cmath>

void MapDrawing::mDrawing(const GameField& playField, const PlacedRobot& player, const std::vector<std::unique_ptr<PlacedRobot>>& enemies, const Factory& factory) {
    std::system("clear"); 
    const Robot& playerRobot = player.getRobot();
    
    std::cout << "Name: " << playerRobot.getName() << " | Rank: " << playerRobot.getRank() << "\n";
    std::cout << "HP: " << playerRobot.getHealth() << " / " << playerRobot.getMaxHealth() << "\n";
    std::cout << "Energy: " << playerRobot.getEnergy() << " / "<< playerRobot.getMaxEnergy() << "\n";
    std::cout << "XP: " << playerRobot.getCurrentXp() << " / "<< playerRobot.getXpToNextRank() << "\n";

    std::cout << "Текущий режим подсветки: [" << getCurrentModeName() << "] (Нажмите V для переключения)\n\n";

    for (int y = 0; y < playField.getHeight(); ++y) {
        for (int x = 0; x < playField.getWidth(); ++x) {

            if (player.getX() == x && player.getY() == y) {
                std::cout << Constants::COLOR_PLAYER << "⚀ " << Constants::COLOR_RESET;
                continue;
            }

            int distX = std::abs(player.getX() - x);
            int distY = std::abs(player.getY() - y);
            int vision = player.getRobot().getVisionRadius();

            bool inLiveVision = (distX <= vision && distY <= vision);
            const Cell& cell = playField.getCell(x, y);
            bool cellIsVisible = inLiveVision || cell.getIsDiscovered();

            bool enemyIsHere = false;
            for (const auto& enemy : enemies) {
                if (enemy->getX() == x && enemy->getY() == y && enemy->getRobot().getHealth() > 0) {
                    if (inLiveVision){
                        if (currentMode == Constants::RenderMode::Enemies) {
                            std::cout << Constants::COLOR_ENEMY << "✦ " << Constants::COLOR_RESET;
                        } else {
                            std::cout << "✦ ";
                        }
                        enemyIsHere = true;
                        break;
                    }
                }
            }
            if (enemyIsHere) continue;


            if ((x == factory.getX() || x == factory.getX() + 1) &&
                 (y == factory.getY() || y == factory.getY() + 1)) {

                if (inLiveVision || cell.getIsDiscovered()) {
                    std::cout << "🀫 ";
                } else {
                    std::cout << ". ";
                }
                continue;
            }

            if (!cellIsVisible) {
                std::cout << ". ";
                continue;
            }

            std::string appliedColor = Constants::COLOR_RESET;

            if (currentMode == Constants::RenderMode::Walls && !cell.getIsWalkable()) {
                appliedColor = Constants::COLOR_WALL;
            }
            else if (currentMode == Constants::RenderMode::Statuses && cell.hasStatuses()) {
                appliedColor = Constants::COLOR_STATUS;
            }


            if (!cell.getIsWalkable()) {
                std::cout << appliedColor << "▲ " << Constants::COLOR_RESET;
            }
            else {
                if (!inLiveVision) {
                    std::cout << ". ";
                } else {
                    if (cell.getMovementCost() > Constants::COST_NORMAL_TERRAIN) {
                        std::cout << appliedColor << "~ " << Constants::COLOR_RESET;
                    } else {
                        if (currentMode == Constants::RenderMode::Statuses && cell.hasStatuses()) {
                            std::cout << appliedColor << "░░" << Constants::COLOR_RESET;
                        } else {
                            std::cout << "  ";
                        }
                    }
                }
            }
        }
        std::cout << "\n";
    }
}


void MapDrawing::switchMode() {
    if (currentMode == Constants::RenderMode::Standard) {
        currentMode = Constants::RenderMode::Enemies;
    } else if (currentMode == Constants::RenderMode::Enemies) {
        currentMode = Constants::RenderMode::Walls;
    } else if (currentMode == Constants::RenderMode::Walls) {
        currentMode = Constants::RenderMode::Statuses;
    } else {
        currentMode = Constants::RenderMode::Standard;
    }
}

std::string MapDrawing::getCurrentModeName() const {
    if (currentMode == Constants::RenderMode::Standard) return "Стандартный";
    if (currentMode == Constants::RenderMode::Enemies)  return "Подсветка врагов";
    if (currentMode == Constants::RenderMode::Walls)    return "Подсветка стен";
    return "Подсветка клеток с аномалиями";
}
