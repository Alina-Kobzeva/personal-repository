#include "MapDrawing.h"
#include <iostream>

void MapDrawing::mDrawing(const GameField& playField, const PlacedRobot& player, const std::vector <PlacedRobot>& enemies, const Factory& factory) {
    std::system("clear");
    const Robot& playerRobot = player.getRobot();
    
    std::cout << "Name: " << playerRobot.getName() << " | Rank: " << playerRobot.getRank() << "\n";
    std::cout << "HP: " << playerRobot.getHealth() << " / " << playerRobot.getMaxHealth() << "\n";
    std::cout << "Energy: " << playerRobot.getEnergy() << " / "<< playerRobot.getMaxEnergy() << "\n";
    std::cout << "XP: " << playerRobot.getCurrentXp() << " / "<< playerRobot.getXpToNextRank() << "\n";

    for (int y = 0; y < playField.getHeight(); ++y) {
        for (int x = 0; x < playField.getWidth(); ++x) {
            if (player.getX() == x && player.getY() == y) {
                // std::cout << "P ";
                std::cout << "⚀ ";


                continue;
            }
            bool enemyIsHere = false;
            for (const auto& enemy : enemies) {
                if (enemy.getX() == x && enemy.getY() == y && enemy.getRobot().getHealth() > 0) {
                    // std::cout << "E ";
                    std::cout << "✦ ";

                    enemyIsHere = true;
                    break;
                }
            }
            if (enemyIsHere) continue;
            const Cell& cell = playField.getCell(x, y);

            if ((x == factory.getX() || x == factory.getX() + 1) &&
                 (y == factory.getY() || y == factory.getY() + 1)) {
                std::cout << "🀫 ";
                continue;
            }

            if (!cell.getIsDiscovered()) {
                // std::cout << "? ";
                std::cout << ". ";

            }
            else if (!cell.getIsWalkable()) {
                std::cout << "▲ ";
            }
            else {
                if (cell.getMovementCost() > 1) {
                    std::cout << "~ ";
                }
                else {
                    // std::cout << ". ";
                    std::cout << "  ";

                }
            }
        }
        std::cout << "\n";
    }
};