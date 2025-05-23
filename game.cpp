#include "game.h"
#include "Tank.h"         
#include <conio.h>
#include <Windows.h>
#include <iostream>
#include <cstring>
#include <fstream>
#include <filesystem>
#include "player.h"
#include <sstream>
#include <cstdlib>
#include <ctime>



using namespace std;

static char board[BOARD_ROWS][BOARD_COLS];
std::vector<std::string> fileNames;


void Game::showInstructions()
{
	clrscr();
	cout << "Instructions:\n";
	cout << "This is a 2-player game. Each player controls a tank using separate keys.\n";
	cout << "\nThe objective of the game is to be the last tank standing.\n";
	cout << "\nYou can eliminate the opponent's tank by firing at them directly,\n or by waiting for them to step on a mine - both of which will result in an instant game over.\n";
	cout << "\nFiring at your opponent's cannon will disable said cannon and leave their tank helpless.\n";
	cout << "\nThe only way to achive a win with a missing cannon is to hope the opposing tank will step on a mine,\n so be mindfull not to lose it!\n";
	cout << "\nEach map's border will be a tunnel,\n which can be traversed by both tanks and tank shells to reach the side opposite to the entry point.\n Use them to your adventage!\n";
	cout << "\nTunnels can sometimes be blocked by walls,\n which can only be cleared by firing 2 tank shells at the same wall.\n";
	cout << "\nPress any key to continue....";
	_getch();
	clrscr();
	cout << "Controls:\n";
	cout << "------------------------------------------------------\n";
	cout << " Action                    | Player 1 | Player 2   \n";
	cout << "---------------------------|----------|------------\n";
	cout << " RIGHT track forward       |    E     |     O      \n";
	cout << " RIGHT track backwards     |    D     |     L      \n";
	cout << " LEFT track forward        |    Q     |     U      \n";
	cout << " LEFT track backwards      |    A     |     J      \n";
	cout << " STAY                      |    S     |     K      \n";
	cout << " Shoot                     |    W     |     I (i)  \n";
	cout << "------------------------------------------------------\n";
	cout << "\nPress any key to continue....";
	_getch();
	clrscr();
	cout << "Symbol guide : \n";
	cout << "------------------------------------------------------\n";
	cout << " Game element        | Corresponding symbol  \n";
	cout << "---------------------|-----------------------\n";
	cout << " Tank                |   O    \n";
	cout << " Cannon              | | or / or \\ depending on direction      \n";
	cout << " Mine                |   @    \n";
	cout << " Wall                |   #    \n";
	cout << " Damaged wall        |   %    \n";
	cout << " Tunnel              | | or -    \n";
	cout << " Tank shell          |   *    \n";
	cout << "\nPress any key to continue....";
	_getch();

}



int Game::showMenu(bool& exit, string& screenChoice) {
	system("cls");
	int choice;
	int gameRunning = false;
	int color = false;

	getAllBoardFileNames(fileNames);


	while (!gameRunning) {
		clrscr();
		cout << "Menu\n";
		cout << "(1) Start a new game\n";
		cout << "(2) Start a new game in color mode\n";
		cout << "(8) Present instructions and keys\n";
		cout << "(9) EXIT\n";
		choice = _getch() - '0';

		switch (choice) {
		case 1:

			cout << "Choose a screen" << endl;
			cin >> screenChoice;
			gameRunning = true;
			clrscr();
			break;
		case 2:
			cout << "Choose a screen" << endl;
			cin >> screenChoice;
			gameRunning = true;
			color = true;
			clrscr();
			break;
		case 8:
			showInstructions();
			break;
		case 9:
			cout << "Exiting the game. Goodbye!\n";
			exit = true;
			return color;
		default:
			cout << "Invalid choice. Please try again.\n";
		}

		cout << "\n";
	}
	return color;
}

bool Game::isWall(int x, int y) {
	return board[y][x] == '#';
}

bool Game::isMine(int x, int y) {
	return board[y][x] == '@';
}



bool Game::initBoard(const std::string& filename, std::map<int, Tank*>& player1_tanks, std::map<int, Tank*>& player2_tanks, Point& legendPos, Player& p1, Player& p2)
{
	const int LEGEND_WIDTH = 20;
	const int LEGEND_HEIGHT = 3;

	bool legendBoxDefined = false;
	int startX = 0;
	int startY = 0;
	std::ifstream screen_file(filename);

	int curr_row = 0;
	int curr_col = 0;
	char c;
	int tanks1_id = 1, tanks2_id = 1;

	while (!screen_file.get(c).eof() && curr_row < GameConfig::HEIGHT) {
		if (c == '\n') {
			if (curr_col < GameConfig::WIDTH) {
				strcpy(board[curr_row] + curr_col, std::string(GameConfig::WIDTH - curr_col - 1, ' ').c_str());
			}
			++curr_row;
			curr_col = 0;
			continue;
		}
		else if (c == '1') {
			Point currPos(curr_col, curr_row);
			int direction = rand() % 8;
			p1.incrementTanks();
			player1_tanks.insert({ tanks1_id++, new Tank(currPos, 1, static_cast<GameConfig::cannonDir>(direction)) });
		}
		else if (c == '2') {
			Point currPos(curr_col, curr_row);
			int direction = rand() % 8;
			p2.incrementTanks();
			player2_tanks.insert({ tanks2_id++, new Tank(currPos, 2, static_cast<GameConfig::cannonDir>(direction)) });
		}
		else if (c == 'L') {
			legendPos.setx(curr_col);
			legendPos.sety(curr_row);

			startX = curr_col - 1;
			startY = curr_row - 1;
			legendBoxDefined = true;

			// Check bounds, skip screen if out of range
			if (startX < 0 || startY < 0 ||
				startX + LEGEND_WIDTH + 2 >= GameConfig::WIDTH ||
				startY + LEGEND_HEIGHT + 2 >= GameConfig::HEIGHT)
			{
				std::cerr << "Skipping screen: Legend box out of bounds in file " << filename << std::endl;
				return false;  
			}

			// Draw legend box borders
			for (int y = startY; y <= startY + LEGEND_HEIGHT + 1; ++y) {
				for (int x = startX; x <= startX + LEGEND_WIDTH + 1; ++x) {
					if (y == startY || y == startY + LEGEND_HEIGHT + 1 ||
						x == startX || x == startX + LEGEND_WIDTH + 1) {
						board[y][x] = '&';
					}
				}
			}
			curr_col++; // Advance cursor
		}
		else if (!legendBoxDefined || !(curr_row >= startY && curr_row <= startY + LEGEND_HEIGHT + 1 &&
			curr_col >= startX && curr_col <= startX + LEGEND_WIDTH + 1)) {
			board[curr_row][curr_col++] = c;
		}
		else {
			curr_col++;  // skip legend area chars
		}
	}

	return true; // success
}


void Game::drawBoard() {
	for (int y = 0; y < BOARD_ROWS; ++y) {
		for (int x = 0; x < BOARD_COLS; ++x) {
			char c = board[y][x];
			if (c == '#' || c == '@' || c == '%' || c== '@' || c=='&') {
				gotoxy(GameConfig::MINX + x, GameConfig::MINY + y);
				cout << c;
			}
		}
	}
}

void Game::drawBoardColor() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	for (int y = 0; y < BOARD_ROWS; ++y) {
		for (int x = 0; x < BOARD_COLS; ++x) {
			char c = board[y][x];
			gotoxy(GameConfig::MINX + x, GameConfig::MINY + y);

			switch (c) {
			case '@':  // Mine
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
				break;
			case '#':  // Wall
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				break;
			case '%':  // Weakened wall
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
				break;
			case '|': case '-':  // Borders
				SetConsoleTextAttribute(hConsole, BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
				break;
			default:
				SetConsoleTextAttribute(hConsole, 7);  // Default
				break;
			}

			cout << c;
		}
	}
	SetConsoleTextAttribute(hConsole, 7); // Reset after draw
}

void Game::printScore(Point legPos) {
	gotoxy(legPos.getX(), legPos.getY());
	cout << "Player 1 Score: " << scoreP1;
	gotoxy(legPos.getX(), legPos.getY()+1);
	cout << "Player 2 Score: " << scoreP2;

}

void Game::pauseGame(std::map <int, Tank*> player1_tanks, std::map <int, Tank*> player2_tanks, bool& running, int color) {
	gotoxy(4, 30);
	cout << "Game paused, press ESC again to continue or X to go back to the main menu";
	char keyPressed = 0;

	int shellColorStep = 0;
	for (auto& [tankID, tankPtr] : player1_tanks) {
		if (tankPtr != nullptr) {
			tankPtr->drawShells(board, color, shellColorStep);
		}
	}

	for (auto& [tankID, tankPtr] : player2_tanks) {
		if (tankPtr != nullptr) {
			tankPtr->drawShells(board, color, shellColorStep);
		}
	}



	while (keyPressed != (char)GameConfig::eKeys::ESC && keyPressed != (char)GameConfig::eKeys::RETURN1 && keyPressed != (char)GameConfig::eKeys::RETURN2)
	{
		if (_kbhit())
			keyPressed = _getch();
	}
	if (keyPressed == (char)GameConfig::eKeys::RETURN1 || keyPressed == (char)GameConfig::eKeys::RETURN2)
		running = false;


	

	for (auto& [tankID, tankPtr] : player1_tanks) {
		if (tankPtr != nullptr) {
			gotoxy(tankPtr->getPosition().getX(), tankPtr->getPosition().getY());
			cout << " ";
			tankPtr->erase(board, color);
		}
	}
	for (auto& [tankID, tankPtr] : player2_tanks) {
		if (tankPtr != nullptr) {
			gotoxy(tankPtr->getPosition().getX(), tankPtr->getPosition().getY());
			cout << " ";
			tankPtr->erase(board, color);
		}
	}
	

	gotoxy(0, GameConfig::HEIGHT + GameConfig::MINY + 1);
	cout << "                                                                         ";

}

void Game::printResult(Player& p1, Player& p2) {
	if (p1.getNumOfTanks()>0 && p2.getNumOfTanks()==0) {
		gotoxy(30, 3);
		cout << "Player 2 died, Player 1 wins!\nPress any key to continue....";
		_getch();
	}

	else if (p2.getNumOfTanks() > 0 && p1.getNumOfTanks() == 0) {
		gotoxy(30, 3);
		cout << "Player 1 died, Player 2 wins!\nPress any key to continue....";
		_getch();
	}
	else if (p1.getNumOfTanks() == 0 && p2.getNumOfTanks() == 0) {
		gotoxy(30, 3);
		cout << "Both players died at the same time, its a tie!\nPress any key to continue....";
		_getch();
	}
}

void Game::handleShells(std::vector<Shell>& shells, Player& p1, Player& p2, bool& running, int color){ //written using chatgpt
	
		for (auto& s : shells) s.move();

		size_t n = shells.size();
		std::vector<bool> collided(n, false);

		// Handle shell vs shell collisions
		for (size_t i = 0; i < n; ++i) {
			for (size_t j = i + 1; j < n; ++j) {
				if (shells[i].position == shells[j].position) {
					collided[i] = collided[j] = true;
				}
			}
		}

		// Check collision of shells with tanks of both players
		for (size_t i = 0; i < n; ++i) {
			if (collided[i]) continue;

			Shell& s = shells[i];
			Point sp = s.position;

			for (Player* player : { &p1, &p2 }) {
				auto& tanksMap = player->getTanksMap();

				for (auto& [id, tank] : tanksMap) {
					if (!tank || !tank->isTankAlive()) continue;

					Point body = tank->getPosition();
					Point cannon = body.add(tank->getPointFromDir(tank->getCannonDir()));

					// Check hit to tank body
					if (sp == body) {
						
						tank->setTankStatus();
						tank->erase(board, color);
						
						
						player->decrementTanks();
						if (id == player->getControlledTank())
							player->changeControlledTank();

						tanksMap.erase(id);

						if (s.player != player->getPlayerID())
							(s.player == 1 ? updateScoreP1(5) : updateScoreP2(5));
						if (player->getNumOfTanks() == 0)
							running = false;
						collided[i] = true;
						break;
					}
					// Check hit to tank cannon
					else if (sp == cannon && tank->isCannonActive()) {
						tank->hitCannon();
						if (s.player != player->getPlayerID())
							(s.player == 1 ? updateScoreP1(2) : updateScoreP2(2));
						collided[i] = true;
						break;
					}
				}
				if (collided[i]) break;
			}

			if (collided[i]) continue;

			// Wall and weakened wall hit detection
			int bx = sp.getX();
			int by = sp.getY();
			if (bx >= 0 && bx < BOARD_COLS && by >= 0 && by < BOARD_ROWS) {
				char& tile = board[by][bx];
				if (tile == '#') {
					tile = '%';
					(s.player == 1 ? updateScoreP1(1) : updateScoreP2(1));
					gotoxy(GameConfig::MINX + bx, GameConfig::MINY + by);
					cout << '%';
					collided[i] = true;
				}
				else if (tile == '%') {
					tile = ' ';
					(s.player == 1 ? updateScoreP1(2) : updateScoreP2(2));
					gotoxy(GameConfig::MINX + bx, GameConfig::MINY + by);
					cout << ' ';
					collided[i] = true;
				}
				else if (tile == '&') {
					collided[i] = true;
				}
			}
		}

		// Keep only surviving shells
		std::vector<Shell> survivors;
		survivors.reserve(n);
		for (size_t i = 0; i < n; ++i) {
			if (!collided[i]) {
				survivors.push_back(shells[i]);
			}
		}
		shells.swap(survivors);

		// Redraw shells
		int shellColorStep = 0;
		for (auto& s : shells)
			s.draw(color, shellColorStep++);
	

}

void Game::getAllBoardFileNames(std::vector<std::string>& vec_to_fill) {
	namespace fs = std::filesystem;
	for (const auto& entry : fs::directory_iterator(fs::current_path())) {
		auto filename = entry.path().filename();
		auto filenameStr = filename.string();
		if (filenameStr.substr(0, 11) == "tanks-game_" && filename.extension() == ".screen") {
			vec_to_fill.push_back(filenameStr);
		}
	}
}

//void Game::run() {
//	bool exit = false;
//	Player p1(1);
//	Player p2(2);
//	std::vector<std::string>filenames;
//	getAllBoardFileNames(filenames);
//	string screenChoice;
//	srand(static_cast<unsigned>(time(0)));
//
//	while (!exit) {
//
//		int color = showMenu(exit, screenChoice);
//		string filename = "tanks-game_" + screenChoice + ".screen";
//		Point legPos;
//		initBoard(filename, p1.getTanksMap(), p2.getTanksMap(), legPos, p1, p2);
//		if (exit)
//			break;
//		if (!color)
//			drawBoard();
//		else
//			drawBoardColor();
//
//		
//
//		vector<Shell> shells;
//		int shellColorStep = 0;
//		for (auto& [tankID, tankPtr] : p1.getTanksMap()) {
//			if (tankPtr != nullptr) {
//				tankPtr->drawShells(board, color, shellColorStep);
//			}
//		}
//
//		for (auto& [tankID, tankPtr] : p2.getTanksMap()) {
//			if (tankPtr != nullptr) {
//				tankPtr->drawShells(board, color, shellColorStep);
//			}
//		}
//		
//
//
//		bool running = true;
//		while (running) {
//			for (auto& [tankID, tankPtr] : p1.getTanksMap()) {
//				if (tankPtr != nullptr) {
//					tankPtr->erase(board, color);
//				}
//			}
//			for (auto& [tankID, tankPtr] : p2.getTanksMap()) {
//				if (tankPtr != nullptr) {
//					tankPtr->erase(board, color);
//				}
//			}
//
//			for (auto& s : shells) s.erase(board);
//
//			if (_kbhit()) {
//				char ch = _getch();
//				if (ch == (char)GameConfig::eKeys::ESC) {
//					pauseGame(p1.getTanksMap(), p2.getTanksMap(), running, color);
//				}
//			}
//
//			if (GetAsyncKeyState('Z') & 0x8000) {
//				if (p1.getControlledTank() != p1.getNumOfTanks())
//					p1.setControlledTank(p1.getControlledTank() + 1);
//				else
//					p1.setControlledTank(1);
//
//			}
//
//			if (GetAsyncKeyState('M') & 0x8000) {
//				if (p2.getControlledTank() != p2.getNumOfTanks())
//					p2.setControlledTank(p2.getControlledTank() + 1);
//				else
//					p2.setControlledTank(1);
//			}
//			auto it1 = p1.getTanksMap().find(p1.getControlledTank());
//			if (it1 != p1.getTanksMap().end() && it1->second != nullptr) {
//				it1->second->checkKeysP1(&p1, board);
//			}
//			auto it2 = p2.getTanksMap().find(p2.getControlledTank());
//			if (it2 != p2.getTanksMap().end() && it2->second != nullptr) {
//				it2->second->checkKeysP2(&p2, board);
//			}
//			if ((GetAsyncKeyState('W') & 0x8000) && it1->second->isCannonActive() && it1->second->getShootCooldown() == 0) {
//				shells.emplace_back(it1->second->getPosition().add(it1->second->getPointFromDir(it1->second->getCannonDir())),
//					it1->second->getPointFromDir(it1->second->getCannonDir()));
//				shells[shells.size() - 1].player = 1;
//				it1->second->setShootCooldown();
//			}
//			if ((GetAsyncKeyState('I') & 0x8000) && it2->second->isCannonActive() && it2->second->getShootCooldown() == 0) {
//				shells.emplace_back(it2->second->getPosition().add(it2->second->getPointFromDir(it2->second->getCannonDir())),
//					it2->second->getPointFromDir(it2->second->getCannonDir()));
//				shells[shells.size() - 2].player = 2;
//
//				it2->second->setShootCooldown();
//			}
//
//			/*it1->second->tick();
//			it2->second->tick();*/
//
//			for (auto& [tankID, tankPtr] : p1.getTanksMap()) {
//				if (tankPtr != nullptr) {
//					if (board[tankPtr->getPosition().getY()][tankPtr->getPosition().getX()] == '@') {
//						p1.decrementTanks();
//						running = false;
//						tankPtr->setTankStatus();
//					}
//				}
//			}
//			for (auto& [tankID, tankPtr] : p2.getTanksMap()) {
//				if (tankPtr != nullptr) {
//					if (board[tankPtr->getPosition().getY()][tankPtr->getPosition().getX()] == '@') {
//						p2.decrementTanks();
//						running = false;
//						tankPtr->setTankStatus();
//					}
//				}
//			}
//
//			for (auto& [tankID, tankPtr] : p1.getTanksMap()) {
//				if (tankPtr != nullptr) {
//					tankPtr->draw(color);
//				}
//			}
//			for (auto& [tankID, tankPtr] : p2.getTanksMap()) {
//				if (tankPtr != nullptr) {
//					tankPtr->draw(color);
//				}
//			}
//
//
//		
//
//			handleShells(shells, p1, p2, running, color);
//
//			
//
//			printScore(legPos);
//
//			/*printResult(p1, p2); */
//			Sleep(100);
//		}
//	}
//}

void Game::run() {
	bool exit = false;
	Player p1(1);
	Player p2(2);
	std::vector<std::string> filenames;
	getAllBoardFileNames(filenames);
	std::sort(filenames.begin(), filenames.end()); // ensure lexicographic order

	string screenChoice;
	srand(static_cast<unsigned>(time(0)));

	int color = showMenu(exit, screenChoice);
	if (exit) return;

	string selectedFilename = "tanks-game_" + screenChoice + ".screen";

	// Find the index of the selected file in the filenames vector
	auto it = std::find(filenames.begin(), filenames.end(), selectedFilename);
	if (it == filenames.end()) {
		std::cout << "Selected screen not found.\n";
		return;
	}

	size_t startIndex = std::distance(filenames.begin(), it);

	// Loop through screens from selected one forward
	for (size_t i = startIndex; i < filenames.size(); ++i) {
		// Reset game state
		p1 = Player(1);
		p2 = Player(2);
		Point legPos;
		vector<Shell> shells;

		bool loaded = initBoard(filenames[i], p1.getTanksMap(), p2.getTanksMap(), legPos, p1, p2);
		if (!loaded) {
			std::cerr << "Screen " << filenames[i] << " skipped due to legend position.\n";
			continue; 
		}

		if (!color)
			drawBoard();
		else
			drawBoardColor();

		bool running = true;
		while (running) {
			
			for (auto& [tankID, tankPtr] : p1.getTanksMap()) {
				if (tankPtr != nullptr) {
					tankPtr->erase(board, color);
				}
			}
			for (auto& [tankID, tankPtr] : p2.getTanksMap()) {
				if (tankPtr != nullptr) {
					tankPtr->erase(board, color);
				}
			}

			for (auto& s : shells) s.erase(board);

			if (_kbhit()) {
				char ch = _getch();
				if (ch == (char)GameConfig::eKeys::ESC) {
					pauseGame(p1.getTanksMap(), p2.getTanksMap(), running, color);
				}
			}

			if (GetAsyncKeyState('Z') & 0x8000) {
				if (p1.getNumOfTanks() != 1) {
					if (p1.getControlledTank() != p1.getNumOfTanks())
						p1.setControlledTank(p1.getControlledTank() + 1);
					else
						p1.setControlledTank(1);
				}
			}

			if (GetAsyncKeyState('M') & 0x8000) {
				if(p2.getNumOfTanks() != 1){
				if (p2.getControlledTank() != p2.getNumOfTanks())
					p2.setControlledTank(p2.getControlledTank() + 1);
				else
					p2.setControlledTank(1);
				}
			}

			if (p1.getControlledTank() != -1) {
				auto it1 = p1.getTanksMap().find(p1.getControlledTank());
				if (it1 != p1.getTanksMap().end() && it1->second != nullptr) {
					it1->second->checkKeysP1(&p1, board);

					if ((GetAsyncKeyState('W') & 0x8000) && it1->second->isCannonActive() && it1->second->getShootCooldown() == 0) {
						shells.emplace_back(it1->second->getPosition().add(it1->second->getPointFromDir(it1->second->getCannonDir())),
							it1->second->getPointFromDir(it1->second->getCannonDir()));
						shells.back().player = 1;
						it1->second->setShootCooldown();
						
					}
				}
				it1->second->tick();
			}

			if (p2.getControlledTank() != -1) {
				auto it2 = p2.getTanksMap().find(p2.getControlledTank());
				if (it2 != p2.getTanksMap().end() && it2->second != nullptr) {
					it2->second->checkKeysP2(&p2, board);

					if ((GetAsyncKeyState('I') & 0x8000) && it2->second->isCannonActive() && it2->second->getShootCooldown() == 0) {
						shells.emplace_back(it2->second->getPosition().add(it2->second->getPointFromDir(it2->second->getCannonDir())),
							it2->second->getPointFromDir(it2->second->getCannonDir()));
						shells.back().player = 2;
						it2->second->setShootCooldown();
						
					}
				}
				it2->second->tick();
			}

	

			auto& tanksMap = p1.getTanksMap();
			for (auto it = tanksMap.begin(); it != tanksMap.end(); ) {
				Tank* tankPtr = it->second;
				if (tankPtr != nullptr) {
					int x = tankPtr->getPosition().getX();
					int y = tankPtr->getPosition().getY();
					if (board[y][x] == '@') {
						tankPtr->setTankStatus();
						tankPtr->erase(board, color);

						if (it->first == p1.getControlledTank())
							p1.changeControlledTank();

						delete tankPtr; 
						it = tanksMap.erase(it); 
						p1.decrementTanks();

						if (p1.getNumOfTanks() == 0)
							running = false;
						continue; 
					}
				}
				++it; 
			}

			auto& tanksMap2 = p2.getTanksMap();
			for (auto it = tanksMap2.begin(); it != tanksMap2.end(); ) {
				Tank* tankPtr = it->second;
				if (tankPtr != nullptr) {
					int x = tankPtr->getPosition().getX();
					int y = tankPtr->getPosition().getY();
					if (board[y][x] == '@') {
						tankPtr->setTankStatus();
						tankPtr->erase(board, color);

						if (it->first == p2.getControlledTank())
							p2.changeControlledTank();

						delete tankPtr; 
						it = tanksMap2.erase(it); 
						p2.decrementTanks();

						if (p2.getNumOfTanks() == 0)
							running = false;
						continue;
					}
				}
				++it; 
			}


			for (auto& [tankID, tankPtr] : p1.getTanksMap()) {
				if (tankPtr != nullptr) {
					tankPtr->draw(color);
				}
			}
			for (auto& [tankID, tankPtr] : p2.getTanksMap()) {
				if (tankPtr != nullptr) {
					tankPtr->draw(color);
				}
			}




			handleShells(shells, p1, p2, running, color);



			printScore(legPos);

			/*printResult(p1, p2); */
			Sleep(100);
		}
		printResult(p1, p2); // Show result before next screen
		clrscr();
	}

	std::cout << "All screens completed.\nPress any key to return to menu...\n";
	_getch();
}