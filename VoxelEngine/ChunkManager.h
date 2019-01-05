#pragma once
#include "World.h"

class ChunkManager {
public:
	ChunkManager();
	~ChunkManager();

private:
	std::unordered_map<ChunkCoord, Chunk*> visibleMap_;
	std::vector<Chunk*> unloadList;
	std::vector<ChunkCoord> loadList;
};

