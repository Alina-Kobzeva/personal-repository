#pragma once
#include "Status.h"
#include <string>

class SlowdownStatus : public Status {
private:
    int duration;
public:
    SlowdownStatus(int turns = 1) : duration(turns) {}

    void update(class Robot& host) override {
        if (duration > 0) { duration--; }
    }

    void merge(const Status& other) override {
        duration += other.getDuration();
    }

    bool canMove() const override {
        return duration <= 0;
    }

    bool canUseAbilities() const override { return true; }
    void onAbilityAttempt() override {}
    int modifyIncomingDamage(int damage) override { return damage; }

    std::string getName() const override {
        return "Замедление (" + std::to_string(duration) + ")";
    }

    int getDuration() const override { return duration; }
    bool isExpired() const override { return duration <= 0; }
};