#include "shell.h"


void Shell::move() {
    position.setx((position.getX() + direction.getX() + GameConfig::WIDTH) % GameConfig::WIDTH);
    position.sety((position.getY() + direction.getY() + GameConfig::HEIGHT) % GameConfig::HEIGHT);
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
