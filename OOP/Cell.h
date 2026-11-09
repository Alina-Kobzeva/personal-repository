#pragma once
#include <vector>
#include <memory>
#include "Status.h"

class Cell {
private:
    bool isWalkable;
    int movementCost;
    bool isDiscovered;

    std::vector<std::unique_ptr<Status>> cellStatuses;

public:
    Cell();

    Cell(bool walkable, int movCost);

    Cell(Cell&&) noexcept = default; 
    Cell& operator=(Cell&&) noexcept = default;
    
    bool getIsWalkable() const;
    void setIsWalkable(bool walkable);

    int getMovementCost() const;
    void setMovementCost(int movCost);

    bool getIsDiscovered() const;
    void setIsDiscovered(bool discovered);

    void addStatus(std::unique_ptr<Status> status) {
        if (cellStatuses.size() < 2 && status) {
            cellStatuses.push_back(std::move(status));
        }
    }
    
    bool hasStatuses() const { 
        return !cellStatuses.empty(); 
    }

    std::vector<std::unique_ptr<Status>>& getCellStatuses() {
        return cellStatuses;
    }

    void removeOneRandomStatus();
}; 