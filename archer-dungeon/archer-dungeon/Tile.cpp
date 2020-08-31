#include "Tile.h"

Tile::Tile() {
}

Tile::~Tile() {
}

void Tile::Initialize(TextureManager::TextureDictionary dictionary, SDL_Renderer* rend, int x, int y, int h, int w, int tid, int cid) {
	std::string textureImg;
	switch (cid) {
		case 0:
			textureImg = "floor.png";
			SetObjectIdOnTile(0);
			//SetContainsId(0, dictionary);
			break;
		case 1:
			textureImg = "wall.png";
			SetObjectIdOnTile(1);
			//SetContainsId(1, dictionary);
			break;
		default:
			textureImg = "wall.png";
			SetObjectIdOnTile(1);
			//SetContainsId(1, dictionary);
			break;

	}

	GameObject* t = new GameObject(dictionary, textureImg, rend, x, y, h, w);
	tile = t;
	tileId = tid;
	containsId = cid;
	contains = NULL;
}