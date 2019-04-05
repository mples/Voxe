#pragma once
#include <unordered_map>
#include "Generation/Generator.h"

class World {
public:
	World(Generator* gen);
	~World();
	
	void setBlock(int x, int y, int z, BlockType type);
	void setChunk(int x, int y, int z, Chunk * chunk);
	Chunk* getChunk(ChunkCoord& coord);
	Chunk* getChunk(int x, int y, int z);
	std::unordered_map<ChunkCoord, Chunk*> & getChunks();
	static ChunkCoord getChunkCoord(int x, int y, int z);
private:
	void setAdjacentChunks(Chunk* chunk, const ChunkCoord& coord);

	std::unordered_map<ChunkCoord, Chunk*> modifiedChunks_;
	Generator* generator_;
};


