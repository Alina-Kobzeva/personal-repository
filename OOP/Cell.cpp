#include "Cell.h"
#include "Constants.h"
#include <cstdlib>

Cell::Cell() : isWalkable(true), movementCost(Constants::COST_NORMAL_TERRAIN), isDiscovered(false) {}

Cell::Cell(bool walkable, int movCost) : isWalkable(walkable), movementCost(movCost), isDiscovered(false) {}

bool Cell::getIsWalkable() const {
    return isWalkable;
}

void Cell::setIsWalkable(bool walkable) {
    isWalkable = walkable;
}

int Cell::getMovementCost() const {
    return movementCost;
}

void Cell::setMovementCost(int cost) {
    if (cost > 0) {
        movementCost = cost;
    }
}

bool Cell::getIsDiscovered() const {
    return isDiscovered;
}

void Cell::setIsDiscovered(bool discovered) {
    isDiscovered = discovered;
}

void Cell::removeOneRandomStatus() {
    if (cellStatuses.empty()) return;
    int indexToRemove = std::rand() % cellStatuses.size();
    cellStatuses.erase(cellStatuses.begin() + indexToRemove);
}