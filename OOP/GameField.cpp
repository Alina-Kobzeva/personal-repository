#include "GameField.h"
#include "Constants.h"

GameField::GameField(int fieldWidth, int fieldHeight) {
    if (fieldWidth < Constants::MIN_FIELD_SIZE) fieldWidth = Constants::MIN_FIELD_SIZE;
    if (fieldWidth > Constants::MAX_FIELD_SIZE) fieldWidth = Constants::MAX_FIELD_SIZE;
    if (fieldHeight < Constants::MIN_FIELD_SIZE) fieldHeight = Constants::MIN_FIELD_SIZE;
    if (fieldHeight > Constants::MAX_FIELD_SIZE) fieldHeight = Constants::MAX_FIELD_SIZE;

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