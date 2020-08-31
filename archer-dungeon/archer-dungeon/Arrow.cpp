#include "Arrow.h"
#include "Grid.h"

Arrow::Arrow(TextureManager::TextureDictionary dictionary, std::string textureImg, SDL_Renderer* rend, int h, int w, Tile* startingTile, Grid* g) {
	int x = startingTile->GetTile()->GetX();
	int y = startingTile->GetTile()->GetY();
	GameObject* arrow = new GameObject(dictionary, textureImg, rend, x, y, h, w);
	arrowObject = arrow;
	isGone = false;
	curTileIndex = startingTile->GetTileId();
	grid = g;
}

Arrow::~Arrow() {
}