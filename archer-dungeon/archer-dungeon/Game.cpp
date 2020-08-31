#include "Game.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "Grid.h"
#include "Player.h"
#include "Chest.h"
#include "Enemy.h"
#include "Arrow.h"

// 0 = Floor, 1 = Wall, 2 = Player, 3 = Chest, 4 = Enemies

// SDL Message box data needed for the lose/win condition popup boxes.
const SDL_MessageBoxButtonData winButtons[] = {
	{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "Next Level" },
};
const SDL_MessageBoxButtonData loseButtons[] = {
	{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "Try Again" },
};
const SDL_MessageBoxColorScheme colorScheme = {
	{ { 255,   0,   0 }, { 0, 255,   0 }, { 255, 255,   0 }, { 0,   0, 255 }, { 255,   0, 255 } }
};
const SDL_MessageBoxData winmessageboxdata = {
	SDL_MESSAGEBOX_INFORMATION, NULL, "Winner!", "You beat this dungeon, but the real treasure has yet to be found!", SDL_arraysize(winButtons), winButtons, &colorScheme
};
const SDL_MessageBoxData losemessageboxdata = {
	SDL_MESSAGEBOX_INFORMATION, NULL, "Game Over!", "Uh-oh! You've been slained, but there's still treasure to be found!", SDL_arraysize(loseButtons), loseButtons, &colorScheme
};
int buttonid;

// The texture dictionary holds ALL textures that have been preloaded into the game
TextureManager::TextureDictionary textureDictionary;

// The player object
Player* player;

// The chest object
Chest* chest;

// The list of all arrows currently on the level
std::vector<Arrow*>* arrowList = new std::vector<Arrow*>();

// The list of all enemies currently on the level
std::vector<Enemy*>* enemyList = new std::vector<Enemy*>();

// The grid/maze ofthe current level
Grid* grid;

Game::Game() {
}

Game::~Game() {
}

// Intialization of the game, this does all SDL Renderering chores first, making sure that all prerequsities are handled
// before we start actually drawing the game. (i.e. initializing sub_systems, rendering, windows, textures)
void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "SDL Subsystems initialized..." << std::endl;

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window) {
			std::cout << "SDL Window initalized..." << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			std::cout << "SDL Renderer intialized..." << std::endl;
		}

		textureDictionary = TextureManager::LoadAllTextures("assets/", renderer);
		if (textureDictionary.size() > 0) {
			std::cout << "SDL textures initialized..." << std::endl;
		}

		isRunning = true;
	}
	else {
		isRunning = false;
	}
}

// This function handles events sent by the user and does a corresponding action.
void Game::handleEvents() {
	SDL_Event event;
	SDL_PollEvent(&event);

	int killedEnemy = -1;
	switch (event.type) {
		// We should check if it's the player's turn before the check for input
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
				case SDLK_UP:
					player->Move(Direction::North);
					break;
				case SDLK_DOWN:
					player->Move(Direction::South);
					break;
				case SDLK_LEFT:
					player->Move(Direction::West);
					break;
				case SDLK_RIGHT:
					player->Move(Direction::East);
					break;
				case SDLK_w:
					// Shoot North
					killedEnemy = player->ShootArrow(Direction::North);
					if (killedEnemy != -1) {
						enemyList->at(killedEnemy)->setIsDead(true);
					};
					break;
				case SDLK_a:
					// Shoot West
					killedEnemy = player->ShootArrow(Direction::West);
					if (killedEnemy != -1) {
						enemyList->at(killedEnemy)->setIsDead(true);
					};
					break;
				case SDLK_s:
					// Shoot South
					killedEnemy = player->ShootArrow(Direction::South);
					if (killedEnemy != -1) {
						enemyList->at(killedEnemy)->setIsDead(true);
					};
					break;
				case SDLK_d:
					// Shoot East
					killedEnemy = player->ShootArrow(Direction::East);
					if (killedEnemy != -1) {
						enemyList->at(killedEnemy)->setIsDead(true);
					};
					break;
				case SDLK_q:
					// Check ammo
					std::cout << "ARROWS: " << player->arrows << std::endl;
					break;
			}
			break;
		case SDL_QUIT:
			isRunning = false;
			break;
		default:
			break;
	}
}

// This function updates any data corresponding to any gameObject (i.e. position)
void Game::update() {
	grid->Update();
	player->GetGameObject()->Update();
	chest->GetGameObject()->Update();
	for (int i = 0; i < enemyList->size(); i++) {
		if (!enemyList->at(i)->getIsDead()) {
			enemyList->at(i)->GetGameObject()->Update();
		}
	}
	for (int j = 0; j < arrowList->size(); j++) {
		arrowList->at(j)->GetGameObject()->Update();
	}
}

// This function clears the screen, then renders all gameObjects.
void Game::render() {
	SDL_RenderClear(renderer);

	grid->Render();
	player->GetGameObject()->Render();
	chest->GetGameObject()->Render();
	for (int i = 0; i < enemyList->size(); i++) {
		if (!enemyList->at(i)->getIsDead()) {
			enemyList->at(i)->GetGameObject()->Render();
		}
	}
	for (int j = 0; j < arrowList->size(); j++) {
		if (!arrowList->at(j)->getIsGone()) {
			arrowList->at(j)->GetGameObject()->Render();
		}
	}

	SDL_RenderPresent(renderer);
}

// Once the game is exited, we need to clean everything.
void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Application cleaned and exiting..." << std::endl;
}

// Called to initialize a level with a maze, player, chest, and enemies.
void Game::InitializeLevel(int level, int mazeModifier, int enemyModifier, int** fileMaze, int height, int width) {
	int realMazeSize = mazeModifier * level;
	int realEnemyCount = (level + enemyModifier) / 2;
	int realArrowCount = realEnemyCount / 2;
	int realStartArrow = 1 + (realArrowCount / 2);

	std::cout << "========================================" << std::endl;
	std::cout << "===        GENERATING LEVEL " << level << "        ===" << std::endl;
	std::cout << "========================================" << std::endl;

	if (fileMaze == NULL) {
		// Initializes the maze with the "realMazeSize"
		std::cout << "Generating random maze..." << std::endl;
		grid = new Grid(width, height);
		grid->Initialize(textureDictionary, "floor.png", renderer);
		grid->GenerateMaze(textureDictionary, realMazeSize);
		std::cout << "Random maze generator finished..." << std::endl;
	}
	else {
		std::cout << "Generating maze from File..." << std::endl;
		grid = new Grid(width, height);
		grid->InitializeFromFile(textureDictionary, "floor.png", renderer, fileMaze);
		std::cout << "Maze generator finished..." << std::endl;
	}

	// Finds an empty tile, and generates the player
	std::cout << "Finding suitable spawn for player..." << std::endl;
	Tile* playerStartTile = grid->GetEmptyTile();
	std::cout << "Spawned player at tile " << playerStartTile->GetTileId() << "..." << std::endl;
	player = new Player(textureDictionary, "player.png", renderer, 16, 16, playerStartTile, grid);
	playerStartTile->SetObjectIdOnTile(2);
	player->arrows = realStartArrow;
	std::cout << "Player has " << player->arrows << " arrow(s)." << std::endl;

	// Find an empty tile, and generates a chest
	std::cout << "Finding suitable spawn for chest" << std::endl;
	Tile* chestStartTile = grid->GetEmptyTile();
	std::cout << "Spawned chest at tile " << chestStartTile->GetTileId() << "..." << std::endl;
	chest = new Chest(textureDictionary, "chest.png", renderer, 16, 16, chestStartTile, grid);
	chestStartTile->SetObjectIdOnTile(3);

	// Clean up the enemyList first
	enemyList->clear();

	// Finds an empty tile, then generates the "realEnemyCount" amount of enemies.
	Tile* enemyStartTile = grid->GetEmptyTile();
	for (int i = 0; i < realEnemyCount; i++) {
		int randEnemy = grid->InitRandNumber(9);
		std::string randEnemyString = std::to_string(randEnemy);
		std::string randEnemyImg = "enemy0" + randEnemyString + ".png";

		std::cout << "Spawned " << randEnemyImg << " at tile " << enemyStartTile->GetTileId() << "..." << std::endl;
		Enemy *enemy = new Enemy(textureDictionary, randEnemyImg, renderer, 16, 16, enemyStartTile, grid);
		enemyList->push_back(enemy);
		enemyStartTile->SetObjectIdOnTile(4 + i);
		enemyStartTile = grid->GetEmptyTile();
	}

	// Clearn up the arrowList first
	arrowList->clear();

	//Finds an empty tile, then generates the "realArrowCount" amount of arrows.
	Tile* arrowStartTile = grid->GetEmptyTile();
	for (int i = 0; i < realArrowCount; i++) {
		std::cout << "Spawned arrow at " << arrowStartTile->GetTileId() << "..." << std::endl;
		Arrow *arrow = new Arrow(textureDictionary, "quiver.png", renderer, 16, 16, arrowStartTile, grid);
		arrowList->push_back(arrow);
		arrowStartTile->SetContains(arrow->GetGameObject());
		arrowStartTile = grid->GetEmptyTile();
	}
}

//Checks if the position of the player is the same exact position of the arrow, if so, then the player gains one more arrow to shoot.
void Game::pickupArrow() {
	for (int i = 0; i < arrowList->size(); i++) {
		if (player->GetCurTileIndex() == arrowList->at(i)->GetCurTileIndex() && !player->isDead && !arrowList->at(i)->getIsGone()) {
			arrowList->at(i)->setIsGone(true);
			player->arrows++;
			std::cout << "ARROW:" << player->arrows << std::endl;
		}
	}
}

// Checks if the position of the player is the same exact position of the chest, if so, then the player won the level.
bool Game::CheckWinConditions() {
	if (player->GetCurTileIndex() == chest->GetCurTileIndex() && !player->isWinner) {
		std::cout << "The player has beaten the level!" << std::endl;
		player->isWinner = true;
		SDL_ShowMessageBox(&winmessageboxdata, &buttonid);
		if (buttonid == -1) {
			std::cout << "User did not press the 'next level' button, closing application..." << std::endl;
			isRunning = false;
		}
		else {
			return true;
		}
	}
	return false;
}

// Checks if the position of the player is the same as any enemy, if so, then lose the game and restart
bool Game::CheckLoseConditions() {
	for (int i = 0; i < enemyList->size(); i++) {
		if (player->GetCurTileIndex() == enemyList->at(i)->GetCurTileIndex() && !player->isDead && !enemyList->at(i)->getIsDead()) {
			std::cout << "The player has died!" << std::endl;
			player->isDead = true;
			SDL_ShowMessageBox(&losemessageboxdata, &buttonid);
			if (buttonid == -1) {
				std::cout << "User did not press the 'next level' button, closing application..." << std::endl;
				isRunning = false;
			}
			else {
				return true;
			}
		}
	}
	return false;
}
