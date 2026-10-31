#include "GameField.h"

GameField::GameField(int fieldWidth, int fieldHeight) {
    if (fieldWidth < 5) fieldWidth = 5;
    if (fieldWidth > 30) fieldWidth = 30;
    if (fieldHeight < 5) fieldHeight = 5;
    if (fieldHeight > 30) fieldHeight = 30;

    width = fieldWidth;
    height = fieldHeight;

    field = std::vector<std::vector<Cell>>(height, std::vector<Cell>(width, Cell()));
}

int GameField::getWidth() const {
    return width;
}

int GameField::getHeight() const {
    return height;
}

Cell& GameField::getCell(int x, int y) {
    if (x >= width || y >= height || x < 0 || y < 0) {
        return field[0][0];
    }
    return field[y][x];
}

const Cell& GameField::getCell(int x, int y) const {
    if (x >= width || y >= height || x < 0 || y < 0) {
        return field[0][0];
    }
    return field[y][x];
}