#include "Enemy.h"
#include "Grid.h"

Enemy::Enemy(TextureManager::TextureDictionary dictionary, std::string textureImg, SDL_Renderer* rend, int h, int w, Tile* startingTile, Grid* g) {
	int x = startingTile->GetTile()->GetX();
	int y = startingTile->GetTile()->GetY();
	GameObject* player = new GameObject(dictionary, textureImg, rend, x, y, h, w);
	enemyObject = player;
	isDead = false;
	curTileIndex = startingTile->GetTileId();
	grid = g;
}

Enemy::~Enemy() {
}

void Enemy::Move(Direction direction) {
	Tile * tile = &grid->GetTileBy(curTileIndex);
	switch (direction) {
	case Direction::North:
		tile = &grid->GetTileBy(curTileIndex - grid->GetWidth());
		if (!grid->IsTileWall(tile) && !grid->IsBorderTile(*tile)) {
			enemyObject->Translate(direction, 32);
			curTileIndex = tile->GetTileId();
		}
		break;
	case Direction::South:
		tile = &grid->GetTileBy(curTileIndex + grid->GetWidth());
		if (!grid->IsTileWall(tile) && !grid->IsBorderTile(*tile)) {
			enemyObject->Translate(direction, 32);
			curTileIndex = tile->GetTileId();
		}
		break;
	case Direction::East:
		tile = &grid->GetTileBy(curTileIndex + 1);
		if (!grid->IsTileWall(tile) && !grid->IsBorderTile(*tile)) {
			enemyObject->Translate(direction, 32);
			curTileIndex = tile->GetTileId();
		}
		break;
	case Direction::West:
		tile = &grid->GetTileBy(curTileIndex - 1);
		if (!grid->IsTileWall(tile) && !grid->IsBorderTile(*tile)) {
			enemyObject->Translate(direction, 32);
			curTileIndex = tile->GetTileId();
		}
		break;
	}
}