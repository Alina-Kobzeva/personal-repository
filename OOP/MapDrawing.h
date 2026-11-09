#pragma once
#include <vector>
#include "GameField.h"
#include "PlacedRobot.h"
#include "Factory.h"
#include <memory>
#include "Constants.h"

class MapDrawing {
private:
    Constants::RenderMode currentMode = Constants::RenderMode::Standard;

public:
    MapDrawing() = default;
    
    void mDrawing(const GameField& playField, const PlacedRobot& player, const std::vector<std::unique_ptr<PlacedRobot>>& enemies, const Factory& factory);

    void switchMode();

    std::string getCurrentModeName() const;
};