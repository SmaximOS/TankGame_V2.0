#include "shell.h"


void Shell::move() {
    if (position.getX() < 1)
    {
        position.setx(GameConfig::WIDTH - 1);
    }
    else if (position.getX() >= GameConfig::WIDTH)
    {
        position.setx(1);
    }
    else position.setx((position.getX() + direction.getX()));
    if (position.getY() < 1)
    {
        position.sety(GameConfig::HEIGHT - 1);
    }
    else if (position.getY() >= GameConfig::HEIGHT)
    {
        position.sety(GameConfig::MINY);
    }
    else position.sety((position.getY() + direction.getY()));
}

void Shell::draw(bool colorMode, int colorStep) const {
    gotoxy(GameConfig::MINX + position.getX(), GameConfig::MINY + position.getY());

    if (colorMode) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        static std::vector<WORD> shellColors = {
            FOREGROUND_RED | FOREGROUND_INTENSITY,
            FOREGROUND_GREEN | FOREGROUND_INTENSITY,
            FOREGROUND_BLUE | FOREGROUND_INTENSITY,
            FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
            FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
            FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
        };

        SetConsoleTextAttribute(hConsole, shellColors[colorStep % shellColors.size()]);
    }

    std::cout << '*';

    if (colorMode) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // Reset
    }
}

void Shell::erase(const char board[GameConfig::HEIGHT][GameConfig::WIDTH]) const {
    int bx = position.getX();
    int by = position.getY();
    char bg = board[by][bx];
    gotoxy(GameConfig::MINX + bx, GameConfig::MINY + by);
    cout << bg;
}