#pragma once
#include <string>

class Ability {
public:
    virtual ~Ability() = default;
    virtual void execute(class PlacedRobot& caster, 
            class GameField& field,
            std::vector<std::unique_ptr<class PlacedRobot>>& enemies) = 0;       
    virtual int getEnergyCost() const = 0;
    virtual void upgrade() = 0;
    virtual std::string getName() const = 0;
};
