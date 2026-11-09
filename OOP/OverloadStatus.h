#pragma once
#include "Status.h"
#include <string>

class OverloadStatus : public Status {
private:
    bool triggeredByAbility;

public:
    OverloadStatus() : triggeredByAbility(false) {}

    void update(class Robot& host) override {}

    void merge(const Status& other) override {}

    bool canMove() const override { return true; }

    bool canUseAbilities() const override { return false; }

    void onAbilityAttempt() override {
        triggeredByAbility = true;
    }

    int modifyIncomingDamage(int damage) override { return damage; }

    std::string getName() const override {
        return "Перегрузка (активна)";
    }

    int getDuration() const override { return 0; }
    bool isExpired() const override { return triggeredByAbility; }
};