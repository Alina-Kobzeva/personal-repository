#pragma once
#include "Ability.h"
#include "Constants.h"
#include <iostream>
#include "PlacedRobot.h"
#include "GameField.h"
#include <string>
#include "ShieldStatus.h"

class Treatment : public Ability {
private:
    int level;
    int healValue;

public:
    Treatment() : level(Constants::HEAL_START_LEVEL),
                  healValue(Constants::HEAL_START_VALUE) {}

    void execute(PlacedRobot& caster, GameField& field, std::vector<std::unique_ptr<PlacedRobot>>& enemies) override {
        Robot& robot = caster.getRobot();

        int currentHp = robot.getHealth();
        int maxHp = robot.getMaxHealth();

        int missingHp = maxHp - currentHp;

        // Уже здоров
        if (missingHp <= 0) {
            robot.addStatus(std::make_unique<ShieldStatus>(healValue));
            std::cout << robot.getName() << " уже имеет полный запас здоровья! Лечение преобразовано в Щит прочностью " << healValue << ".\n";
        }

        // Слишком много здоровья -> уходим в щит
        else if (healValue > missingHp) {
            int overhealAmount = healValue - missingHp;
            robot.heal(missingHp);
            robot.addStatus(std::make_unique<ShieldStatus>(overhealAmount));
            std::cout << robot.getName() << " полностью вылечен! Избыточное лечение перешло в Щит прочностью " << overhealAmount << ".\n";
        }

        // if (robot.getHealth() >= Constants::PLAYER_MAX_HP) {
        //     std::cout << "Здоровье робота уже на максимуме!\n";
        //     return;
        // }

        // Обычное лечение
        else {
            robot.heal(healValue);
            std::cout << robot.getName() << " был вылечен. Текущее здоровье: " << robot.getHealth() << "\n";
        }
    }

    int getEnergyCost() const override{
        return Constants::ENERGY_FOR_TREATMENT;
    }

    void upgrade() override{
        level++;
        healValue += Constants::HEAL_UPGRADE_STEP;
        std::cout << "Способность лечения улучшена до уровня " << level
                  << "! Эффективность : " << healValue << "\n";
    }   

    std::string getName() const override {
        return "Лечение (ур." + std::to_string(level) + ")";
    }
};