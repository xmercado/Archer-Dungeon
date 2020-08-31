#pragma once
#include "GameObject.h"

class Tile {
public:
	Tile();
	~Tile();

	void SetContains(GameObject* obj) { contains = obj; }
	void SetContainsId(int cid, TextureManager::TextureDictionary dictionary) { 
		SDL_Texture* t = NULL;
		containsId = cid; 
		switch (cid) {
			case 0:
				t = TextureManager::GetTexture(dictionary, "floor.png");
				SetObjectIdOnTile(0);
				break;
			case 1:
				t = TextureManager::GetTexture(dictionary, "wall.png");
				SetObjectIdOnTile(0);
				break;
		}
		tile->SetTexture(t);
	}
	GameObject* GetContains() { return contains; }
	GameObject* GetTile() { return tile; }

	void SetObjectIdOnTile(int id) { objectIdOnTile = id; }
	int GetObjectIdOnTile() { return objectIdOnTile; }

	int GetTileId() { return tileId; }
	void SetTileId(int i) { tileId = i; }
	int GetContainsId() { return containsId; }

	void Initialize(TextureManager::TextureDictionary dictionary, SDL_Renderer* rend, int x, int y, int h, int w, int tid, int cid);

private:
	GameObject* tile;
	GameObject* contains;
	int tileId;
	int containsId;
	int objectIdOnTile;
};
