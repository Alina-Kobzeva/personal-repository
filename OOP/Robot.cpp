#include "Robot.h"
#include "Ability.h"
#include "Status.h"
#include <algorithm>

Robot::Robot(std::string robotName, int maxHp, int maxEn, int attack, int robotSpeed, int vision, bool enemy)
    : name(robotName), health(maxHp), maxHealth(maxHp), energy(maxEn), maxEnergy(maxEn), attackPower(attack), currentXp(0),
    xpToNextRank(100), rank(1), speed(robotSpeed), visionRadius(vision), isEnemy(enemy), currentMovePoints(robotSpeed){}


std::string Robot::getName() const { return name; }
int Robot::getHealth() const { return health; }
int Robot::getMaxHealth() const { return maxHealth; }
int Robot::getAttackPower() const { return attackPower; }
int Robot::getEnergy() const { return energy; }
int Robot::getMaxEnergy() const { return maxEnergy; }
int Robot::getCurrentXp() const { return currentXp; }
int Robot::getXpToNextRank() const { return xpToNextRank; }
int Robot::getRank() const { return rank; }
int Robot::getSpeed() const { return speed; }
int Robot::getVisionRadius() const { return visionRadius; }
bool Robot::getIsEnemy() const { return isEnemy; }

// Получение урона
void Robot::takeDamage(int amount) {
    if (amount <= 0) return;
    
    int finalDamage = amount;
    for (auto& status : activeStatuses) {
        finalDamage = status->modifyIncomingDamage(finalDamage);
    }

    health -= finalDamage;
    if (health < 0) {
        health = 0;
    }
}

// Лечение
void Robot::heal(int amount) {
    if (amount <= 0) return;
    health += amount;
    if (health > maxHealth) {
        health = maxHealth;
    }
}

// Трата энергии
void Robot::spendEnergy(int amount) {
    if (amount <= 0) return;
    energy -= amount;
    if (energy < 0) {
        energy = 0;
    }
}

// Восстановление энергии
void Robot::restoreEnergy(int amount) {
    if (amount <= 0) return;
    energy += amount;
    if (energy > maxEnergy) {
        energy = maxEnergy;
    }
}

// Повышение опыта
void Robot::gainXp(int amount) {
    if (amount <= 0) return;
    currentXp += amount;
    checkRankUp();
}

void Robot::checkRankUp() {
    while (currentXp >= xpToNextRank) {
        currentXp -= xpToNextRank;
        rank++;
        xpToNextRank = static_cast<int>(xpToNextRank * 1.5);

        maxHealth += 25;
        health = maxHealth;

        maxEnergy += 15;
        energy = maxEnergy;

        attackPower += 10;
    }
}

void Robot::interact(Robot& other) {
    if (this->isEnemy != other.getIsEnemy()) {
        other.takeDamage(this->attackPower);
        this->gainXp(30);
    }

    else {
        int healthAmount = this->maxHealth / 4;
        other.heal(healthAmount);
    }
}

int Robot::getCurrentMovePoints() const{
    return currentMovePoints;
}

void Robot::resetMovePoints() {
    currentMovePoints = speed;
}

void Robot::spendMovePoints(int amount) {
    if (amount <= 0) return;
    currentMovePoints -= amount;
    if (currentMovePoints < 0) {
        currentMovePoints = 0;
    }
}

void Robot::tickStatuses() {
    for (auto& status : activeStatuses) {
        status->update(*this);
    }

    activeStatuses.erase(
        std::remove_if(activeStatuses.begin(), activeStatuses.end(),
            [](const std::unique_ptr<Status>& status) {
                return status->isExpired();
            }),
        activeStatuses.end()
    );
}

void Robot::addStatus(std::unique_ptr<Status> newStatus) {
    if (!newStatus) return;

    for (auto& currentStatus : activeStatuses) {
        if (currentStatus->getName().substr(0, 4) == newStatus->getName().substr(0, 4)) {
            currentStatus->merge(*newStatus);
            return;
        }
    }
    activeStatuses.push_back(std::move(newStatus));
}
