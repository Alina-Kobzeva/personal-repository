#pragma once

class Cell {
private:
    bool isWalkable;

    int movementCost;

    bool isDiscovered;

public:
    Cell();

    Cell(bool walkable, int movCost);

    bool getIsWalkable() const;
    void setIsWalkable(bool walkable);

    int getMovementCost() const;
    void setMovementCost(int movCost);

    bool getIsDiscovered() const;
    void setIsDiscovered(bool discovered);
}; 