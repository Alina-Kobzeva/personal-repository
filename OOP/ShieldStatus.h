#pragma once
#include "Status.h"
#include <string>
#include <iostream>

class ShieldStatus : public Status {
private:
    int shieldAmount;
public:
    ShieldStatus(int amount = 20) : shieldAmount(amount) {}

    void update(class Robot& host) override {}

    void merge(const Status& other) override {
        shieldAmount += other.getDuration();
        std::cout << "Прочность энергетического щита увеличилась! Текущая емкость: " << shieldAmount << "\n";
    }

    bool canMove() const override { return true; }
    bool canUseAbilities() const override { return true; }
    void onAbilityAttempt() override {}

    int modifyIncomingDamage(int damage) override { 
        if (shieldAmount <= 0) return damage;

        if (shieldAmount >= damage) {
            shieldAmount -= damage;
            std::cout << "Щит поглотил " << damage << " ед. урона! (Остаток щита: " << shieldAmount << ")\n";
            return 0;
        } else {
            int penetratedDamage = damage - shieldAmount;
            std::cout << "Щит полностью разрушен! Поглощено " << shieldAmount << " урона. Пробитие: " << penetratedDamage << "\n";
            shieldAmount = 0;
            return penetratedDamage;
        }
    }

    std::string getName() const override {
        return "Щит (Защита: " + std::to_string(shieldAmount) + ")";
    }

    int getDuration() const override { return shieldAmount; }
    bool isExpired() const override { return shieldAmount <= 0; }
};