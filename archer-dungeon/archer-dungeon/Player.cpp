#include "Player.h"
#include "Grid.h"

Player::Player(TextureManager::TextureDictionary dictionary, std::string textureImg, SDL_Renderer* rend, int h, int w, Tile* startingTile, Grid* g) {
	int x = startingTile->GetTile()->GetX();
	int y = startingTile->GetTile()->GetY();
	GameObject* player = new GameObject(dictionary, textureImg, rend, x, y, h, w);
	playerObject = player;
	isDead = false;
	curTileIndex = startingTile->GetTileId();
	grid = g;
	arrows = 0;
}

Player::~Player() {
}

int Player::ShootArrow(Direction direction) {
	if (arrows > 0) {
		int initialCurTileIndex = curTileIndex;
		Tile * tile = &grid->GetTileBy(curTileIndex);
		switch (direction) {
		case Direction::North:
			while (tile->GetObjectIdOnTile() < 4 && tile->GetObjectIdOnTile() != 1) {
				tile = &grid->GetTileBy(curTileIndex - grid->GetWidth());
				curTileIndex = tile->GetTileId();
			}

			if (tile->GetObjectIdOnTile() >= 4) {
				std::cout << "Killed Enemy" << std::endl;
				arrows--;
				std::cout << "ARROWS: " << arrows << std::endl;
				int enemyTile = tile->GetObjectIdOnTile() - 4;
				tile->SetObjectIdOnTile(0);
				curTileIndex = initialCurTileIndex;
				return enemyTile;
			}
			curTileIndex = initialCurTileIndex;
			break;
		case Direction::South:
			while (tile->GetObjectIdOnTile() < 4 && tile->GetObjectIdOnTile() != 1) {
				tile = &grid->GetTileBy(curTileIndex + grid->GetWidth());
				curTileIndex = tile->GetTileId();
			}

			if (tile->GetObjectIdOnTile() >= 4) {
				std::cout << "Killed Enemy" << std::endl;
				arrows--;
				std::cout << "ARROWS: " << arrows << std::endl;
				int enemyTile = tile->GetObjectIdOnTile() - 4;
				tile->SetObjectIdOnTile(0);
				curTileIndex = initialCurTileIndex;
				return enemyTile;
			}
			curTileIndex = initialCurTileIndex;
			break;
		case Direction::East:
			while (tile->GetObjectIdOnTile() < 4 && tile->GetObjectIdOnTile() != 1) {
				tile = &grid->GetTileBy(curTileIndex + 1);
				curTileIndex = tile->GetTileId();
			}

			//FIXME
			// Enemy that are dead are still counted towards here
			// Two enemies that are next together doesn't kill the one in front. 
			if (tile->GetObjectIdOnTile() >= 4) {
				std::cout << "Killed Enemy" << std::endl;
				arrows--;
				std::cout << "ARROWS: " << arrows << std::endl;
				int enemyTile = tile->GetObjectIdOnTile() - 4;
				tile->SetObjectIdOnTile(0);
				curTileIndex = initialCurTileIndex;
				return enemyTile;
			}
			curTileIndex = initialCurTileIndex;
			break;
		case Direction::West:
			while (tile->GetObjectIdOnTile() < 4 && tile->GetObjectIdOnTile() != 1) {
				tile = &grid->GetTileBy(curTileIndex - 1);
				curTileIndex = tile->GetTileId();
			}

			if (tile->GetObjectIdOnTile() >= 4) {
				std::cout << "Killed Enemy" << std::endl;
				arrows--;
				std::cout << "ARROWS: " << arrows << std::endl;
				int enemyTile = tile->GetObjectIdOnTile() - 4;
				tile->SetObjectIdOnTile(0);
				curTileIndex = initialCurTileIndex;
				return enemyTile;
			}
			curTileIndex = initialCurTileIndex;
			break;
		}
	}
	return -1;
}

void Player::Move(Direction direction) {
	Tile * tile = &grid->GetTileBy(curTileIndex);
	tile->SetObjectIdOnTile(0);
	switch (direction) {
		case Direction::North:
			tile = &grid->GetTileBy(curTileIndex - grid->GetWidth());
			tile->SetObjectIdOnTile(2);
			if (!grid->IsTileWall(tile) && !grid->IsBorderTile(*tile)) {
				playerObject->Translate(direction, 32);
				curTileIndex = tile->GetTileId();
			}
			break;
		case Direction::South:
			tile = &grid->GetTileBy(curTileIndex + grid->GetWidth());
			tile->SetObjectIdOnTile(2);
			if (!grid->IsTileWall(tile) && !grid->IsBorderTile(*tile)) {
				playerObject->Translate(direction, 32);
				curTileIndex = tile->GetTileId();
			}
			break;
		case Direction::East:
			tile = &grid->GetTileBy(curTileIndex + 1);
			tile->SetObjectIdOnTile(2);
			if (!grid->IsTileWall(tile) && !grid->IsBorderTile(*tile)) {
				playerObject->Translate(direction, 32);
				curTileIndex = tile->GetTileId();
			}
			break;
		case Direction::West:
			tile = &grid->GetTileBy(curTileIndex - 1);
			tile->SetObjectIdOnTile(2);
			if (!grid->IsTileWall(tile) && !grid->IsBorderTile(*tile)) {
				playerObject->Translate(direction, 32);
				curTileIndex = tile->GetTileId();
			}
			break;
	}
}