#pragma once

#include "SDL.h"
#include "SDL_image.h"

#include <vector>
#include <map>
#include <random>
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>

namespace fs = std::experimental::filesystem;

class Game {

public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	void handleEvents();
	void update();
	void render();
	void clean();

	void InitializeLevel(int level, int mazeModifier, int enemyModifier, int** fileMaze, int height, int width);

	void pickupArrow();

	bool CheckWinConditions();
	bool CheckLoseConditions();

	bool running() { return isRunning; }

private:
	bool isRunning;
	SDL_Window *window;
	SDL_Renderer *renderer;
};