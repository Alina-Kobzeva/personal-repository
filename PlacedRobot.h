#pragma once
#include "Robot.h"

class PlacedRobot {
private:
    Robot& robot;
    int x;
    int y;
    
public: 
    PlacedRobot(Robot& myRobot, int startX, int startY);

    int getX() const;
    int getY() const;

    void setX(int newX);
    void setY(int newY);

    Robot& getRobot();
    const Robot& getRobot() const;
};