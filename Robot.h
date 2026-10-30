#pragma once
#include <string>

class Robot {
private:
    std::string name;

    int health;
    int maxHealth;

    int attackPower;

    int energy;
    int maxEnergy;
    
    int currentXp;
    int xpToNextRank;
    int rank;

    int speed;
    int visionRadius;
    bool isEnemy;
    int currentMovePoints;

    void checkRankUp();

public:
    Robot(std::string robotName, int maxHp, int maxEn, int attack, int robotSpeed, int vision, bool enemy);

    std::string getName() const;
    int getHealth() const;
    int getMaxHealth() const;
    int getAttackPower() const;
    int getEnergy() const;
    int getMaxEnergy() const;
    int getCurrentXp() const;
    int getXpToNextRank() const;
    int getRank() const;
    int getSpeed() const;
    int getVisionRadius() const;
    bool getIsEnemy() const;

    void takeDamage(int amount);
    void heal(int amount);
    void spendEnergy(int amount);
    void restoreEnergy(int amount);
    void gainXp(int amount);

    void interact(Robot& other);

    int getCurrentMovePoints() const;
    void resetMovePoints();
    void spendMovePoints(int amount);
};