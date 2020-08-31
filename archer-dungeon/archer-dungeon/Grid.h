#pragma once
#include "Tile.h"

class Grid {
public:
	Grid(int w, int h);
	~Grid();

	void Initialize(TextureManager::TextureDictionary dictionary, std::string textureImg, SDL_Renderer* rend);
	void InitializeFromFile(TextureManager::TextureDictionary dictionary, std::string textureImg, SDL_Renderer* rend, int** fileInput);
	void Print();

	void Update();
	void Render();

	void GenerateMaze(TextureManager::TextureDictionary dictionary, int size);

	std::vector<Tile>* GetMaze() { return &maze; }

	Tile* GetEmptyTile();
	bool IsTileWall(Tile* t);
	Tile GetTileBy(int id);
	int GetTileIndex(int id);
	int GetWidth() { return width; }
	bool IsBorderTile(Tile t);
	int InitRandNumber(int max);

private:
	int width;
	int height;

	Tile **grid;

	std::vector<Tile> maze;
	std::vector<Tile> borders;

	// Helper functions for the GenerateMaze() function
	std::vector<Tile>* GetFrontierTiles(std::vector<Tile>* frontier, Tile t);
	int CreateFloor(Tile t);

};