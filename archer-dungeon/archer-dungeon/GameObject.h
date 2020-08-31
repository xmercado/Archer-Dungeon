#pragma once

#include "Game.h"
#include "TextureManager.h"
#include "Direction.h"

class GameObject {
public:
	GameObject();
	GameObject(TextureManager::TextureDictionary dictionary, std::string textureImg, SDL_Renderer* rend, int x, int y, int h, int w);
	~GameObject();

	void SetTexture(SDL_Texture* t) { texture = t; }

	void Update();
	void Render();
	void Translate(Direction direction, int dist);

	int GetX() { return xpos; }
	int GetY() { return ypos; }

	bool isActive;
private:

	int xpos;
	int ypos;

	int height;
	int width;

	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
	SDL_Renderer* renderer;
};
