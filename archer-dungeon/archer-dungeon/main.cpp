#include "Game.h"

Game *game = nullptr;
int curLevel = 1;
int mazeModifier = 5;
int enemyModifier = 1;

int row, col;

char menuInput = '0';

void getFileMaze() {
	std::string fileName, line;
	int x;
	int** maze = 0;
	maze = new int*[18]; //NOTE: can change '18'
	for (int h = 0; h < 18; h++) {
		maze[h] = new int[18];
	};
	std::ifstream fileIn;
	//open file
	std::cout << "Enter file path: ";
	std::cin >> fileName;
	fileIn.open(fileName);
	if (fileIn.fail()) {
		std::cout << "File cannot open" << std::endl;
		return;
	}
	//read file
	while (fileIn.good()) {
		while (getline(fileIn, line)) {
			std::istringstream stream(line);
			col = 0;
			while (stream >> x) {
				maze[row][col] = x;
				col++;
			}
			row++;
		}
	}
	//display file
	std::cout << "rows: " << row << std::endl;
	std::cout << "columns: " << col << std::endl;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			std::cout << maze[i][j] << " ";
		}
		std::cout << std::endl;
	}

	game->init("Archer Dungeon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 576, 576, false);
	game->InitializeLevel(curLevel, mazeModifier, enemyModifier, maze, col, row);
}

int main(int argc, char* argv[])
{
	int** maze;
	game = new Game();

	std::cout << "============ Welcome to Archer Dungeon ============" << std::endl << std::endl;
	std::cout << "============ MENU " << std::endl;
	std::cout << "1 - Load .txt maze file" << std::endl;
	std::cout << "2 - Play randomly generated maze" << std::endl;
	std::cout << "3 - Quit" << std::endl << std::endl;

	std::cin >> menuInput;
	while (menuInput != '-') {
		switch (menuInput) {
			case '1':
				getFileMaze();
				menuInput = '-';
				break;
			case '2':
				game->init("Archer Dungeon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 576, 576, false);
				game->InitializeLevel(curLevel, mazeModifier, enemyModifier, NULL, 18, 18);
				menuInput = '-';
				break;
			case '3':
				game->clean();
				return 0;
		}
	}


	// Game loop, it will continuous run as long as the "isRunning" flag is true.
	while (game->running()) {
		// Game is TURN-BASED, meaning we need to have a queue for list of player/enemies turns
		// This continouos loop should check for whose turn it is, then run enemies, then wait for user input
		if (game->CheckWinConditions()) {
			curLevel++;
			game->InitializeLevel(curLevel, mazeModifier, enemyModifier, NULL, 18, 18);
		};
		if (game->CheckLoseConditions()) {
			curLevel = 1;
			game->InitializeLevel(curLevel, mazeModifier, enemyModifier, NULL, 18, 18);
		}
		game->pickupArrow();
		game->handleEvents();
		game->update();
		game->render();
	}

	game->clean();
	return 0;
}