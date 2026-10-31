#pragma once
#include <vector>
#include "GameField.h"
#include "PlacedRobot.h"
#include "Factory.h"

class MapDrawing {
public:
    void mDrawing(const GameField& playField, const PlacedRobot& player, const std::vector <PlacedRobot>& enemies, const Factory& factory);
};