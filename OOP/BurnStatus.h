#pragma once
#include "Status.h"
#include <string>
#include <algorithm>
#include "Robot.h"
#include "Constants.h"

class BurnStatus : public Status {
private:
    int duration;
    int damagePerTurn;

public:
    BurnStatus(int turns = Constants::BURN_TURNS, int damage = Constants::BURN_DAMAGE) : duration(turns), damagePerTurn(damage) {}

    void update(Robot& host) override {
        if (duration > 0) {
            duration--;
            host.takeDamage(damagePerTurn);
        }
    }

    void merge(const Status& other) override {
        duration = std::max(duration, other.getDuration());
    }

    bool canMove() const override { return true; }
    bool canUseAbilities() const override { return true; }
    void onAbilityAttempt() override {}
    int modifyIncomingDamage(int damage) override { return damage; }

    std::string getName() const override {
        return "Горение (" + std::to_string(duration) + " х., -" + std::to_string(damagePerTurn) + " ОЗ)";
    }

    int getDuration() const override { return duration; }
    bool isExpired() const override { return duration <= 0; }
};