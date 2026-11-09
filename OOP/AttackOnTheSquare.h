#pragma once
#include "Ability.h"
#include "Constants.h"
#include "PlacedRobot.h"
#include "GameField.h"
#include <iostream>
#include "OverloadStatus.h"

class AttackOnTheSquare : public Ability {
private:
    int level;
    int radius;
    int damage;

public:
    AttackOnTheSquare() : level(Constants::AOS_START_LEVEL),
                          radius(Constants::AOS_RADIUS),
                          damage(Constants::AOS_START_DAMAGE) {}

    void execute(PlacedRobot& caster, GameField& field, std::vector<std::unique_ptr<PlacedRobot>>& enemies) override {
                // Список дел:
                // пройтись по клеткам в радиусе
                // если есть враг -> удар
                // доьавить счётчик задетых врагов   

        int cx = caster.getX();
        int cy = caster.getY();
        int hitCnt = 0;
        
        for (int x = cx - radius; x <= cx + radius; ++x) {
            for (int y = cy - radius; y <= cy + radius; ++y) {
                if (x == cx && y == cy) continue;

                for(auto& enemy : enemies) {
                    if (enemy->getX() == x && enemy->getY() == y && enemy->getRobot().getHealth() > 0) {
                        enemy->getRobot().takeDamage(damage);
                        enemy->getRobot().addStatus(std::make_unique<OverloadStatus>());
                        hitCnt++;
                    }
                }
            }
        }
        if (hitCnt == 0) {
            std::cout << "Никто из врагов не был задет.\n";
        } else {
            std::cout << "Было задето " << hitCnt << " врагов.\n";
        }
    }

    int getEnergyCost() const override {
        return Constants::ENERGY_FOR_AOS_ATTACK;
    }

    void upgrade() override {
        level++;
        radius += 1;
        std::cout << "Атака по площади улучшена! Новый радиус поражения: " << radius << "\n";
    }

    std::string getName() const override {
        return "Удар по площади (ур. " + std::to_string(level) + ")";
    }
};