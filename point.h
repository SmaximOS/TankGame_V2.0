#pragma once
#include "gameConfig.h"

class Point {
    int x, y;
public:
    Point(int x = GameConfig::MINX + 1, int y = GameConfig::MINY + 1) : x(x), y(y) {}

    void setx(int x) { this->x = x; }
    void sety(int y) { this->y = y; }

    int getX() const { return x; }
    int getY() const { return y; }


    bool isEqual(const Point& other) const {
        return x == other.x && y == other.y;
    }

    Point add(const Point& other) const {
        return Point(x + other.x, y + other.y);
    }

    void move(GameConfig::eKeys key);

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

