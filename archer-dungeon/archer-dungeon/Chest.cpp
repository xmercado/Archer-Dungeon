#include "Chest.h"
#include "Grid.h"

Chest::Chest(TextureManager::TextureDictionary dictionary, std::string textureImg, SDL_Renderer* rend, int h, int w, Tile* startingTile, Grid* g) {
	int x = startingTile->GetTile()->GetX();
	int y = startingTile->GetTile()->GetY();
	GameObject* chest = new GameObject(dictionary, textureImg, rend, x, y, h, w);
	chestObject = chest;
	curTileIndex = startingTile->GetTileId();
	grid = g;
}

Chest::~Chest() {
}