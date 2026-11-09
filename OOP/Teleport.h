#pragma once
#include "Ability.h"
#include "PlacedRobot.h"
#include "GameField.h"
#include <iostream>
#include <cmath>
#include "Constants.h"

class Teleport : public Ability {
private:
    int level;
    int maxRange;
    int currentEnergyCost;

public:
    Teleport() : level(Constants::TELEPORT_START_LEVEL),
                 maxRange(Constants::TELEPORT_START_RANGE),
                 currentEnergyCost(Constants::ENERGY_FOR_TELEPORTATION) {}
                 
    int getEnergyCost() const override {
        return currentEnergyCost;
    }
    
    void execute(PlacedRobot& caster, GameField& field, std::vector<std::unique_ptr<PlacedRobot>>& enemies) override {
        int changeX, changeY;
        std::cout << "Введите координаты (x y) для телепортации, текущий радиус телепортации " << maxRange - 1 << ": ";
        std::cin >> changeX >> changeY;
        
        int targetX = caster.getX() + changeX;
        int targetY = caster.getY() + changeY;

        if (targetX < 0 || targetX >= field.getWidth() || targetY < 0 || targetY >= field.getHeight()) {
            std::cout << "Ошибка координат телепортации, выход за пределы поля.\n";
            return;
        }

        if (!field.getCell(targetX, targetY).getIsWalkable()) {
            std::cout << "Это непроходимая клетка.\n";
            return;
        }

        double distance = std::sqrt(std::pow(changeX, 2) + std::pow(changeY, 2));
        if (distance > maxRange) {
            std::cout << "Вы пока не можете телепортироваться на такую дистанцию.\n";
            return;
        }
        
        caster.setX(targetX);
        caster.setY(targetY);
        std::cout << "Робот успешно телепортировался на координаты ( " << targetX << " " << targetY << ")!\n";
    }

    void upgrade() override {
        level++;
        currentEnergyCost -= Constants::TELEPORT_ENERGY_UPDATE; 
        if (currentEnergyCost < 5) currentEnergyCost = 5;
        std::cout << "Телепортация улучшена! Затраты энергии снижены до: " << currentEnergyCost << "\n";
    }

    std::string getName() const override {
        return "Телепортация (ур." + std::to_string(level) + ")";
    }
};