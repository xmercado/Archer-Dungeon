#pragma once

#include "GameObject.h"
#include "Grid.h"

class Player {
public:
	Player(TextureManager::TextureDictionary dictionary, std::string textureImg, SDL_Renderer* rend, int h, int w, Tile* startingTile, Grid* g);
	~Player();

	GameObject* GetGameObject() { return playerObject; }
	void Move(Direction direction);
	int ShootArrow(Direction direction);
	int GetCurTileIndex() { return curTileIndex; }

	bool isDead;
	bool isWinner;

	int arrows;

private:
	GameObject* playerObject;
	int curTileIndex;
	Grid* grid;
};