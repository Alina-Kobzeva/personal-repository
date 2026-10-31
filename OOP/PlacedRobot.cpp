#include "PlacedRobot.h"

PlacedRobot::PlacedRobot(Robot& myRobot, int startX, int startY)
    : robot(myRobot), x(startX), y(startY) {}

int PlacedRobot::getX() const {
    return x;
}

int PlacedRobot::getY() const {
    return y;
}

void PlacedRobot::setX(int newX) {
    x = newX;
}

void PlacedRobot::setY(int newY) {
    y = newY;
}

Robot& PlacedRobot::getRobot() {
    return robot;
}

const Robot& PlacedRobot::getRobot() const {
    return robot;
}