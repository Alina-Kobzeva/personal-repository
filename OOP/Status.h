#pragma once
#include <string>

class Status {
public:
    virtual ~Status() = default;
    virtual void update(class Robot& host) = 0;

    virtual void merge(const Status& other) = 0;

    virtual bool canMove() const = 0;
    virtual bool canUseAbilities() const = 0;

    virtual void onAbilityAttempt() = 0;

    virtual int modifyIncomingDamage(int damage) = 0;

    virtual std::string getName() const = 0;
    virtual int getDuration() const = 0;
    virtual bool isExpired() const = 0;
};