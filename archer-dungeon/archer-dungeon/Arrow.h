#include "GameObject.h"
#include "Grid.h"

class Arrow {
public:
	Arrow(TextureManager::TextureDictionary dictionary, std::string textureImg, SDL_Renderer* rend, int h, int w, Tile* startingTile, Grid* g);
	~Arrow();

	GameObject* GetGameObject() { return arrowObject; }
	int GetCurTileIndex() { return curTileIndex; }

	void setIsGone(bool b) { isGone = b; }
	bool getIsGone() { return isGone; }
private:
	bool isGone;
	GameObject * arrowObject;
	int curTileIndex;
	Grid* grid;
};