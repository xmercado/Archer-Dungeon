#include "Grid.h"

std::random_device rd;
std::mt19937 rng(rd());

Grid::Grid(int w, int h) {
	width = w;
	height = h;

	grid = new Tile*[height];
	for (int i = 0; i < height; i++) {
		grid[i] = new Tile[width];
	}
}

Grid::~Grid() {
}

void Grid::Initialize(TextureManager::TextureDictionary dictionary, std::string textureImg, SDL_Renderer* rend) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			// Hard coded the size and location of tiles
			grid[j][i].Initialize(dictionary, rend, i * 32, j * 32, 16, 16, (i + (j * width)), 1);
			if (i == 0 || i == height - 1) {
				borders.push_back(grid[j][i]);
			} 
			else if (j == 0 || j == width - 1) {
				borders.push_back(grid[j][i]);
			}
			else {
				maze.push_back(grid[j][i]);
			}
		}
	}
}

void Grid::InitializeFromFile(TextureManager::TextureDictionary dictionary, std::string textureImg, SDL_Renderer* rend, int** fileInput) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			// Hard coded the size and location of tiles
			grid[j][i].Initialize(dictionary, rend, i * 32, j * 32, 16, 16, (i + (j * width)), fileInput[j][i]);
			if (i == 0 || i == height - 1) {
				borders.push_back(grid[j][i]);
			}
			else if (j == 0 || j == width - 1) {
				borders.push_back(grid[j][i]);
			}
			else {
				maze.push_back(grid[j][i]);
				if (maze.at(maze.size() - 1).GetObjectIdOnTile() == 0) {
					maze.at(maze.size() - 1).SetContainsId(0, dictionary);
				}
				else {
					maze.at(maze.size() - 1).SetContainsId(1, dictionary);
				}
			}
		}
	}
}

void Grid::Print() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			std::cout << grid[j][i].GetContainsId() << " ";
		}
		std::cout << std::endl;
	}
}

void Grid::Update() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			grid[j][i].GetTile()->Update();
		}
	}
}

void Grid::Render() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			grid[j][i].GetTile()->Render();
		}
	}
}

bool Grid::IsTileWall(Tile* t) {
	if (t->GetContainsId() == 1)
		return true;

	return false;
}

void Grid::GenerateMaze(TextureManager::TextureDictionary dictionary, int size) {
	std::vector<Tile>* tileList = new std::vector<Tile>();
	std::vector<Tile>* frontier = new std::vector<Tile>();

	int counter = 0;

	std::uniform_int_distribution<int> cellRand(0, maze.size() - 1);
	int rand = cellRand(rng);
	Tile startingTile = maze.at(rand);

	maze.at(rand).SetContainsId(0, dictionary);

	tileList->push_back(startingTile);
	frontier = GetFrontierTiles(frontier, startingTile);

	while (frontier->size() != 0 && counter < size) {
		std::uniform_int_distribution<int> frontierRand(0, frontier->size() - 1);
		int fRand = frontierRand(rng);
		Tile curTile = frontier->at(fRand);
		int newFloor = CreateFloor(curTile);
		int newFloorIndex = GetTileIndex(newFloor);
		int frontierIndex = GetTileIndex(curTile.GetTileId());

		std::cout << "Smashing down tile #" << newFloorIndex << " and " << frontierIndex << std::endl;
		maze.at(newFloorIndex).SetContainsId(0, dictionary);
		maze.at(frontierIndex).SetContainsId(0, dictionary);

		frontier = GetFrontierTiles(frontier, curTile);
		frontier->erase(frontier->begin() + fRand);
		counter++;
	}
}

std::vector<Tile>* Grid::GetFrontierTiles(std::vector<Tile>* frontier, Tile t) {
	int startTid = t.GetTileId();

	Tile northTile = GetTileBy(startTid - 2);
	Tile southTile = GetTileBy(startTid + 2);
	Tile eastTile = GetTileBy(startTid + (2 * height));
	Tile westTile = GetTileBy(startTid - (2 * height));

	if (!IsBorderTile(northTile) && northTile.GetContainsId() == 1)
		frontier->push_back(northTile);
	if (!IsBorderTile(southTile) && southTile.GetContainsId() == 1)
		frontier->push_back(southTile);
	if (!IsBorderTile(eastTile) && eastTile.GetContainsId() == 1)
		frontier->push_back(eastTile);
	if (!IsBorderTile(westTile) && westTile.GetContainsId() == 1)
		frontier->push_back(westTile);

	return frontier;
}

int Grid::CreateFloor(Tile t) {
	int connectorTileId = -1;
	int startTid = t.GetTileId();

	Tile northTile = GetTileBy(startTid - 2);
	Tile southTile = GetTileBy(startTid + 2);
	Tile eastTile = GetTileBy(startTid + (2 * height));
	Tile westTile = GetTileBy(startTid - (2 * height));

	while (connectorTileId == -1) {
		std::uniform_int_distribution<int> neighborRand(0, 3);
		int nRand = neighborRand(rng);
		if (nRand == 0 && !IsBorderTile(northTile) && northTile.GetContainsId() == 0)
			connectorTileId = northTile.GetTileId() + 1;
		else if (nRand == 1 && !IsBorderTile(southTile) && southTile.GetContainsId() == 0)
			connectorTileId = southTile.GetTileId() - 1;
		else if (nRand == 2 && !IsBorderTile(eastTile) && eastTile.GetContainsId() == 0)
			connectorTileId = eastTile.GetTileId() - height;
		else if (nRand == 3 && !IsBorderTile(westTile) && westTile.GetContainsId() == 0)
			connectorTileId = westTile.GetTileId() + height;
	}

	return connectorTileId;
}

bool Grid::IsBorderTile(Tile t) {
	int tid = t.GetTileId();

	for (int i = 0; i < borders.size(); i++) {
		if (borders.at(i).GetTileId() == tid) {
			return true;
		}
	}
	return false;
}

Tile Grid::GetTileBy(int id) {
	for (int i = 0; i < maze.size(); i++) {
		if (maze.at(i).GetTileId() == id) {
			return maze.at(i);
		}
	}

	Tile falseTile = Tile();
	falseTile.SetTileId(0);
	return falseTile;
}

int Grid::GetTileIndex(int id) {
	for (int i = 0; i < maze.size(); i++) {
		if (maze.at(i).GetTileId() == id) {
			return i;
		}
	}
	return 999;
}

Tile* Grid::GetEmptyTile() {
	Tile* tile = &maze.at(0);
	while (tile == NULL || tile->GetObjectIdOnTile() != 0 || tile->GetContainsId() != 0) {
		std::uniform_int_distribution<int> startTileRand(0, maze.size() - 1);
		int randStartIndex = startTileRand(rng);
		tile = &maze.at(randStartIndex);
	}
	return tile;
}

int Grid::InitRandNumber(int max) {
	std::uniform_int_distribution<int> rand(0, max);
	return rand(rng);
}