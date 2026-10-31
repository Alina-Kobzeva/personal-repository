#pragma once
#include "Cell.h"
#include <vector>

class GameField {
private:
    int width;
    int height;

    std::vector <std::vector <Cell>> field;

    public:
    GameField(int fieldWidth, int fieldHeight);

    int getWidth() const;
    int getHeight() const;

    Cell& getCell(int x, int y);
    const Cell& getCell(int x, int y) const;
};