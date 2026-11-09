#pragma once
#include "Ability.h"
#include "Constants.h"
#include "PlacedRobot.h"
#include "GameField.h"
#include <iostream>
#include <cmath>
#include "BurnStatus.h"

class FarAttack : public Ability {
private:
    int level;
    int maxRange;
    int damage;

public:
    FarAttack() : level(Constants::RANGED_START_LEVEL),
                  maxRange(Constants::RANGED_START_RANGE),
                  damage(Constants::RANGED_START_DAMAGE) {}

    void execute(PlacedRobot& caster, GameField& field, std::vector<std::unique_ptr<PlacedRobot>>& enemies) override {
                    // Список дел:
                    // считать x y
                    // проверить смещение
                    // проверить границы
                    // проверить растояне
                    // пробежаться по вектору роботов
                    // если совпадают координаты -> атака
                    // если ничего нет -> промах

        int changeX, changeY;
        std::cout << "Введите координаты (x y) для дальнего удара, текущая дистанция:" << maxRange << ": ";
        std::cin >> changeX >> changeY;
        int targetX = caster.getX() + changeX;
        int targetY = caster.getY() + changeY;
        
        if (targetX < 0 || targetX >= field.getWidth() || targetY < 0 || targetY >= field.getHeight()) {
            std::cout << "Ошибка координат дальней атаки, выход за пределы поля.\n";
            return;
        }

        double distance = std::sqrt(std::pow(changeX, 2) + std::pow(changeY, 2));
        if (distance > maxRange) {
            std::cout << "Вы пока не можете атаковать на такую дистанцию.\n";
            return;
        }

        bool hit = false;
        for (auto& enemy : enemies) {
            if (enemy->getX() == targetX && enemy->getY() == targetY && enemy->getRobot().getHealth() > 0) {
                enemy->getRobot().takeDamage(damage);
                enemy->getRobot().addStatus(std::make_unique<BurnStatus>(Constants::BURN_TURNS, Constants::BURN_DAMAGE));
                std::cout << "Попадание во врага " << enemy->getRobot().getName() << "Он загорелся!" 
                          << "\nНанесено урона: " << damage
                          << ". Оставшееся HP врага: " << enemy->getRobot().getHealth() << ".\n";
                hit = true;
                break;
            }
        }

        if (!hit) {
            std::cout << "Мимо, враг не был задет.\n";
        }
    }

    int getEnergyCost() const override {
        return Constants::ENERGY_FOR_RANGED_ATTACK;
    }

    void upgrade() override {
        level++;
        damage += Constants::RANGED_DAMAGE_UPGRADE;
        std::cout << "Дальняя атака улучшена до уровня " << level
                  << "! Новый урон: " << damage << "\n";
    }

    std::string getName() const override {
        return "Дальняя атака (ур. " + std::to_string(level) + ")";
    }

};