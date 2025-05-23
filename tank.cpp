#include "Tank.h"
#include "GameConfig.h"
#include <iostream>
#include <windows.h>
#include "general.h"
#include "game.h"
using namespace std;

Point Tank::getPosition() const {
    return pos;
}


void Tank::draw(int colorMode) const {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (colorMode) {
        if (playerID == 1)
            SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        else if (playerID == 2)
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    }

    gotoxy(GameConfig::MINX + pos.getX(), GameConfig::MINY + pos.getY());
    cout << 'O';

    if (isCannonActive_) {
        Point tip = pos.add(getPointFromDir(cannonDir));
        gotoxy(GameConfig::MINX + tip.getX(), GameConfig::MINY + tip.getY());
        cout << getCannonChar(cannonDir);
    }

    if (colorMode)
        SetConsoleTextAttribute(hConsole, 7); // reset
}



void Tank::erase(const char board[GameConfig::HEIGHT][GameConfig::WIDTH], int color) const {
    // 1) Erase tank body
    {
        int x = pos.getX();
        int y = pos.getY();
        char bg = board[y][x];

        gotoxy(GameConfig::MINX + x, GameConfig::MINY + y);

        if (color) {
            HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


            if (bg == '#') SetConsoleTextAttribute(hConsole, 100); // dark gray wall
            else if (bg == '%') SetConsoleTextAttribute(hConsole, 100); // same as wall
            else if (bg == '@') SetConsoleTextAttribute(hConsole, 41); // red mine
            else if (bg == '-' || bg == '|') SetConsoleTextAttribute(hConsole, 47); // borders
            else SetConsoleTextAttribute(hConsole, 7); // default

            cout << bg;

            SetConsoleTextAttribute(hConsole, 7); // reset to default
        }
        else {
            cout << bg;
        }
    }


    if (isCannonActive_) {
        Point p = getPointFromDir(cannonDir);
        int bx = pos.getX() + p.getX();
        int by = pos.getY() + p.getY();
        char bg = board[by][bx];
        gotoxy(GameConfig::MINX + bx,
            GameConfig::MINY + by);
        cout << bg;
    }
}

char Tank::getCannonChar(GameConfig::cannonDir dir) const {
    if (dir == GameConfig::cannonDir::NORTH || dir == GameConfig::cannonDir::SOUTH) return '|';
    if (dir == GameConfig::cannonDir::EAST || dir == GameConfig::cannonDir::WEST) return '-';
    if (dir == GameConfig::cannonDir::NORTH_EAST || dir == GameConfig::cannonDir::SOUTH_WEST) return '/';
    if (dir == GameConfig::cannonDir::NORTH_WEST || dir == GameConfig::cannonDir::SOUTH_EAST) return '\\';
    return '?';
}
Point Tank::getPointFromDir(GameConfig::cannonDir dir) const {
    Point p;

    switch (dir)
    {
    case GameConfig::cannonDir::NORTH:
        p.setx(0);
        p.sety(-1);
        break;

    case GameConfig::cannonDir::NORTH_EAST:
        p.setx(1);
        p.sety(-1);
        break;

    case GameConfig::cannonDir::EAST:
        p.setx(1);
        p.sety(0);
        break;
    case GameConfig::cannonDir::SOUTH_EAST:
        p.setx(1);
        p.sety(1);
        break;

    case GameConfig::cannonDir::SOUTH:
        p.setx(0);
        p.sety(1);
        break;

    case GameConfig::cannonDir::SOUTH_WEST:
        p.setx(-1);
        p.sety(1);
        break;

    case GameConfig::cannonDir::WEST:
        p.setx(-1);
        p.sety(0);
        break;

    case GameConfig::cannonDir::NORTH_WEST:
        p.setx(-1);
        p.sety(-1);
        break;
    }

    return p;
}



void Tank::move(int x, int y, char board[GameConfig::HEIGHT][GameConfig::WIDTH]) {
    Point p(x, y);
    char tile = board[pos.getY() + y][pos.getX() + x];
    if (tile == '#' || tile == '%'|| tile=='&') {
        p.setx(0);
        p.sety(0);
    }
    pos = pos.add(p);

    if (pos.getX() < 1)
        pos.setx(GameConfig::WIDTH - 1);
    else if (pos.getX() >= GameConfig::WIDTH)
        pos.setx(GameConfig::MINX);


    if (pos.getY() < 1)
        pos.sety(GameConfig::HEIGHT - 1);
    else if (pos.getY() >= GameConfig::HEIGHT)
        pos.sety(GameConfig::MINY);
}

void Tank::setDirection(GameConfig::cannonDir newDir) {
    cannonDir = newDir;
}


void Tank::tick() {
    if (shootCooldown > 0)
        --shootCooldown;
}

void Tank::hitCannon() {
    isCannonActive_ = false;
}

bool Tank::isAt(const Point& p) const {
    return pos.isEqual(p);
}

int Tank::getPlayerID() const {
    return playerID;
}

void Tank::checkKeysP1(Player* p1, char board[GameConfig::HEIGHT][GameConfig::WIDTH]) { //large portion written with help from chatgpt
    // 1) Read raw key states
    bool lf = GetAsyncKeyState('Q') & 0x8000;  // left?track forward
    bool rf = GetAsyncKeyState('E') & 0x8000;  // right?track forward
    bool lb = GetAsyncKeyState('A') & 0x8000;  // left?track backward
    bool rb = GetAsyncKeyState('D') & 0x8000;  // right?track backward
    bool stay = GetAsyncKeyState('S') & 0x8000;  // stay
    bool shoot = GetAsyncKeyState('W') & 0x8000; // shoot

    // 2) STOP immediately on stay
    if (stay) {
        isMoving = false;
        velocity = Point(0, 0);
        return;
    }

    // 3) ROTATION IN PLACE (no movement this frame)
    // 3a) Diagonal (8?step) rotations:
    if (rf && !lf && !lb && !rb) {
        // RIGHT track forward ONLY ? CCW 1 step
        int idx = (static_cast<int>(cannonDir) + 7) % 8;
        GameConfig::cannonDir nextDir = static_cast<GameConfig::cannonDir>(idx);

        if (canRotateTo(nextDir, board)) {
            cannonDir = nextDir;
        }
        return;
    }
    else if (lf && !rf && !lb && !rb) {
        // LEFT track forward ONLY ? CW 1 step
        int idx = (static_cast<int>(cannonDir) + 1) % 8;
        GameConfig::cannonDir nextDir = static_cast<GameConfig::cannonDir>(idx);

        if (canRotateTo(nextDir, board)) {
            cannonDir = nextDir;
        }
        return;
    }

    // 3b) Cardinal (4?step) rotations:
    if (rf && lb && !lf && !rb) {
        // RIGHT forward + LEFT backward ? CCW 2 steps
        int idx = (static_cast<int>(cannonDir) + 6) % 8;
        GameConfig::cannonDir nextDir = static_cast<GameConfig::cannonDir>(idx);

        if (canRotateTo(nextDir, board)) {
            cannonDir = nextDir;
        }
        return;
    }
    else if (lf && rb && !rf && !lb) {
        // LEFT forward + RIGHT backward ? CW 2 steps
        int idx = (static_cast<int>(cannonDir) + 2) % 8;
        GameConfig::cannonDir nextDir = static_cast<GameConfig::cannonDir>(idx);

        if (canRotateTo(nextDir, board)) {
            cannonDir = nextDir;
        }
        return;
    }

    // 4) START COASTING if both tracks are driven
    if (lf && rf) {
        velocity = getPointFromDir(cannonDir);
        isMoving = true;
    }
    else if (lb && rb) {
        Point d = getPointFromDir(cannonDir);
        velocity = Point(-d.getX(), -d.getY());
        isMoving = true;
    }

    // 5) COAST if flagged
    if (isMoving) {
        move(velocity.getX(), velocity.getY(), board);
    }


}

bool Tank::canRotateTo(GameConfig::cannonDir newDir, char board[GameConfig::HEIGHT][GameConfig::WIDTH]) const {
    Point offset = getPointFromDir(newDir);
    Point cannonTip = pos.add(offset);

    int x = cannonTip.getX();
    int y = cannonTip.getY();


    if (x < 0 || x >= GameConfig::WIDTH || y < 0 || y >= GameConfig::HEIGHT)
        return false;

    char tile = board[y][x];
    return (tile != '#' && tile != '%'&& tile != '&');
}



void Tank::checkKeysP2(Player* p2, char board[GameConfig::HEIGHT][GameConfig::WIDTH]) { //identical to P1 
    bool lf = GetAsyncKeyState('U') & 0x8000;
    bool rf = GetAsyncKeyState('O') & 0x8000;
    bool lb = GetAsyncKeyState('J') & 0x8000;
    bool rb = GetAsyncKeyState('L') & 0x8000;
    bool stay = GetAsyncKeyState('K') & 0x8000;
    bool shoot = GetAsyncKeyState('I') & 0x8000;



    // 2) STOP immediately on stay
    if (stay) {
        isMoving = false;
        velocity = Point(0, 0);
        return;
    }

    // 3) ROTATION IN PLACE (no movement this frame)
    // 3a) Diagonal (8?step) rotations:
    if (rf && !lf && !lb && !rb) {
        // RIGHT track forward ONLY ? CCW 1 step
        int idx = (static_cast<int>(cannonDir) + 7) % 8;
        GameConfig::cannonDir nextDir = static_cast<GameConfig::cannonDir>(idx);

        if (canRotateTo(nextDir, board)) {
            cannonDir = nextDir;
        }
        return;
    }
    else if (lf && !rf && !lb && !rb) {
        // LEFT track forward ONLY ? CW 1 step
        int idx = (static_cast<int>(cannonDir) + 1) % 8;
        GameConfig::cannonDir nextDir = static_cast<GameConfig::cannonDir>(idx);

        if (canRotateTo(nextDir, board)) {
            cannonDir = nextDir;
        }
        return;
    }

    // 3b) Cardinal (4?step) rotations:
    if (rf && lb && !lf && !rb) {
        // RIGHT forward + LEFT backward ? CCW 2 steps
        int idx = (static_cast<int>(cannonDir) + 6) % 8;
        GameConfig::cannonDir nextDir = static_cast<GameConfig::cannonDir>(idx);

        if (canRotateTo(nextDir, board)) {
            cannonDir = nextDir;
        }
        return;
    }
    else if (lf && rb && !rf && !lb) {
        // LEFT forward + RIGHT backward ? CW 2 steps
        int idx = (static_cast<int>(cannonDir) + 2) % 8;
        GameConfig::cannonDir nextDir = static_cast<GameConfig::cannonDir>(idx);

        if (canRotateTo(nextDir, board)) {
            cannonDir = nextDir;
        }
        return;
    }

    // 4) START COASTING if both tracks are driven
    if (lf && rf) {
        velocity = getPointFromDir(cannonDir);
        isMoving = true;
    }
    else if (lb && rb) {
        Point d = getPointFromDir(cannonDir);
        velocity = Point(-d.getX(), -d.getY());
        isMoving = true;
    }

    // 5) COAST if flagged
    if (isMoving) {
        move(velocity.getX(), velocity.getY(), board);
    }
}



void Tank::drawShells(char board[GameConfig::HEIGHT][GameConfig::WIDTH], bool colorMode, int& colorStep) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    static std::vector<WORD> shellColors = {
        FOREGROUND_RED | FOREGROUND_INTENSITY,
        FOREGROUND_GREEN | FOREGROUND_INTENSITY,
        FOREGROUND_BLUE | FOREGROUND_INTENSITY,
        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
        FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
        FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
    };

    for (int i = 0; i < MAX_SHELLS; ++i) {
        if (shellActive[i]) {
            const Point& pos = shellPositions[i];

            int x = pos.getX() - GameConfig::MINX;
            int y = pos.getY() - GameConfig::MINY;

            if (x >= 0 && x < BOARD_COLS && y >= 0 && y < BOARD_ROWS) {
                if (colorMode) {
                    SetConsoleTextAttribute(hConsole, shellColors[colorStep % shellColors.size()]);
                }

                gotoxy(GameConfig::MINX + x, GameConfig::MINY + y);
                std::cout << '*';

                if (colorMode) {
                    SetConsoleTextAttribute(hConsole, 7); // reset
                }

                colorStep++; // next shell gets next color
            }
        }
    }
}




