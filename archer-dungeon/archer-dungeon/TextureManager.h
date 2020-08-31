#pragma once

#include "Game.h"

class TextureManager {
public:
	typedef std::map<std::string, SDL_Texture*> TextureDictionary;

	static SDL_Texture* LoadTexture(const char* fileName, SDL_Renderer* renderer);
	static TextureDictionary LoadAllTextures(const char* directory, SDL_Renderer* renderer);
	static SDL_Texture* GetTexture(TextureDictionary dictionary, std::string key);
};
