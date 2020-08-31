#include "TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* fileName, SDL_Renderer* renderer) {
	SDL_Surface* tempSurface = IMG_Load(fileName);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	SDL_FreeSurface(tempSurface);

	return texture;
}

TextureManager::TextureDictionary TextureManager::LoadAllTextures(const char* directory, SDL_Renderer* renderer) {
	TextureDictionary tmpDictionary;

	std::cout << "Started loading textures..." << std::endl;
	for (auto& p : fs::directory_iterator(directory)) {
		std::string pathString = p.path().string();
		const char* path = &pathString[0u];

		std::string::size_type loc = pathString.find("\\");
		std::string nameString = pathString.substr(loc + 1, pathString.length() - loc);

		SDL_Texture* tmp = LoadTexture(path, renderer);
		tmpDictionary.insert(std::pair<std::string, SDL_Texture*>(nameString, tmp));

		std::cout << "Loading texture from " << path << "..." << std::endl;
	}
	std::cout << "Completed loading all textures..." << std::endl;

	return tmpDictionary;
}

SDL_Texture* TextureManager::GetTexture(TextureManager::TextureDictionary dictionary, std::string key) {
	TextureDictionary::const_iterator pos = dictionary.find(key);
	SDL_Texture* texture = NULL;

	if (pos == dictionary.end()) {
		texture = GetTexture(dictionary, "error.png");
		return texture;
	}
	else {
		texture = pos->second;
		return texture;
	}

}