#pragma once
#include "Point.h"
#include "gameConfig.h"
#include "general.h"
#include <iostream>
#include <Windows.h>
#include <vector>

using namespace std;

class Shell {
public:
    Point position;
    Point direction;
    int player;

    Shell(Point pos, Point dir) : position(pos), direction(dir) {}
    void move();
    void draw(bool colorMode, int colorStep) const;
    void erase(const char board[GameConfig::HEIGHT][GameConfig::WIDTH]) const;

};