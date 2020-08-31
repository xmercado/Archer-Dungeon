#pragma once

#include "GameObject.h"
#include "Grid.h"

class Enemy {
public:
	Enemy(TextureManager::TextureDictionary dictionary, std::string textureImg, SDL_Renderer* rend, int h, int w, Tile* startingTile, Grid* g);
	~Enemy();

	GameObject* GetGameObject() { return enemyObject; }
	void Move(Direction direction);
	int GetCurTileIndex() { return curTileIndex; }

	void setIsDead(bool b) { isDead = b; }
	bool getIsDead() { return isDead; }

private:
	int id;
	bool isDead;
	GameObject* enemyObject;
	int curTileIndex;
	Grid* grid;
};