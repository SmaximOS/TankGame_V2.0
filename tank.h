#pragma once
#include "Point.h"
#include <vector>
#include "Shell.h"
#include "gameConfig.h"

#define MAX_SHELLS 3
class Player;

class Tank {
private:
    Point pos;
    GameConfig::cannonDir cannonDir;
    bool isAlive = true;
    bool isMoving = false;
    bool isCannonActive_ = true;
    int shootCooldown = 0;
    int playerID;
    Point velocity{ 0,0 };
    Point shellPositions[MAX_SHELLS];
    Point shellDirections[MAX_SHELLS];
    bool shellActive[MAX_SHELLS];
    int tankID;


public:
    Tank(Point position, int id, GameConfig::cannonDir dir)
        : pos(position), playerID(id), cannonDir(dir)
    {
        for (int i = 0; i < MAX_SHELLS; ++i) {
            shellActive[i] = false;
            shellPositions[i] = pos; // optional, just to init
            shellDirections[i] = Point(0, 0); // optional, safe default
        }
    }

    bool isCannonActive() const { return isCannonActive_; }
    bool isTankAlive() const { return isAlive; }
    GameConfig::cannonDir getCannonDir() const { return cannonDir; }
    int getShootCooldown() const { return shootCooldown; }
    void setShootCooldown() { shootCooldown = 5; }
    Point getPosition() const;
    void draw(int colorMode) const;
    void erase(const char board[GameConfig::HEIGHT][GameConfig::WIDTH], int color) const;
    void move(int x, int y, char board[GameConfig::HEIGHT][GameConfig::WIDTH]);
    void setDirection(GameConfig::cannonDir newDir);
    char getCannonChar(GameConfig::cannonDir dir)  const;
    void tick();
    void hitCannon();
    bool isAt(const Point& p) const;
    void setTankStatus() { isAlive = false; }
    int getPlayerID() const;
    void checkKeysP1(Player* p1, char board[GameConfig::HEIGHT][GameConfig::WIDTH]);
    void checkKeysP2(Player* p2, char board[GameConfig::HEIGHT][GameConfig::WIDTH]);
    Point getPointFromDir(GameConfig::cannonDir dir) const;
    void drawShells(char board[GameConfig::HEIGHT][GameConfig::WIDTH], bool colorMode, int& colorStep);
    bool canRotateTo(GameConfig::cannonDir newDir, char board[GameConfig::HEIGHT][GameConfig::WIDTH]) const;
    bool isMovingStat() const {return isMoving;}
    void setMoving(bool movingStat) { isMoving = movingStat; }
    void setCannonDir(GameConfig::cannonDir newDir) { cannonDir = newDir; }
    GameConfig::cannonDir getCannonDir() { return cannonDir; };


};


