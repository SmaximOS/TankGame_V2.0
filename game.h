#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include "general.h"
#include "gameConfig.h"
#include "tank.h"
#include <vector>
#include <string>
#include <map>






class Game
{
	int scoreP1 = 0, scoreP2 = 0;

public:
	Game() {};
	void run();
	void drawBoard();
	void drawBoardColor();
	void showInstructions();
	bool initBoard(const std::string& filename, std::map <int , Tank*>& player1_tanks, std::map <int, Tank*>& player2_tanks, Point& legendPos, Player& p1, Player& p2);
	int showMenu(bool& exit, string& screenChoice);
	bool isWall(int x, int y);
	bool isMine(int x, int y);
	void pauseGame(std::map <int, Tank*> player1_tanks, std::map <int, Tank*> player2_tanks, bool& running, int color);
	void printResult(Player& p1, Player& p2);
	void handleShells(std::vector<Shell>& shells, Player& p1, Player& p2, bool& running, int color);
	void updateScoreP1(int score) { this->scoreP1 += score; }
	void updateScoreP2(int score) { this->scoreP2 += score; }
	void printScore(Point legPos);
	void getAllBoardFileNames(std::vector<std::string>& vec_to_fill);


};

