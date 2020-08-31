#pragma once

#include "GameObject.h"
#include "Grid.h"

class Chest {
public:
	Chest(TextureManager::TextureDictionary dictionary, std::string textureImg, SDL_Renderer* rend, int h, int w, Tile* startingTile, Grid* g);
	~Chest();

	GameObject* GetGameObject() { return chestObject; }
	int GetCurTileIndex() { return curTileIndex; }

private:
	GameObject* chestObject;
	int curTileIndex;
	Grid* grid;
};