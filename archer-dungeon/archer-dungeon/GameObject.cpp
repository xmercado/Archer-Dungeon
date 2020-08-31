#include "GameObject.h"
#include "TextureManager.h"
#include "Direction.h"

GameObject::GameObject() {
}

GameObject::GameObject(TextureManager::TextureDictionary dictionary, std::string textureImg, SDL_Renderer* rend, int x, int y, int h, int w) {
	texture = TextureManager::GetTexture(dictionary, textureImg);
	renderer = rend;
	xpos = x;
	ypos = y;
	height = h;
	width = w;
}

GameObject::~GameObject() {
}

void GameObject::Update() {
	srcRect.h = height;
	srcRect.w = width;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.x = xpos;
	destRect.y = ypos;
	destRect.w = srcRect.w * 2;
	destRect.h = srcRect.h * 2;
}

void GameObject::Render() {
	SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}

void GameObject::Translate(Direction direction, int dist) {
	switch (direction) {
		case Direction::North:
			ypos -= dist;
			break;
		case Direction::South:
			ypos += dist;
			break;
		case Direction::East:
			xpos += dist;
			break;
		case Direction::West:
			xpos -= dist;
			break;
	}
}