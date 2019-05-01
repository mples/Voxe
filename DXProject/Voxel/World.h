#pragma once
#include <unordered_map>
#include "../Generation/Generator.h"

class World {
public:
	World(Generator* gen);
	~World();
	
	void setBlock(int x, int y, int z, BlockType type);
	void setChunk(int x, int y, int z, Chunk * chunk);
	Chunk* getChunk(ChunkCoord& coord);
	Chunk* getChunk(int x, int y, int z);
	void generateChunk(BlockType blocks[Chunk::DIM][Chunk::DIM][Chunk::DIM], int x, int y, int z);
	void generateChunk(BlockType blocks[Chunk::DIM][Chunk::DIM][Chunk::DIM], ChunkCoord coord);
	std::unordered_map<ChunkCoord, Chunk*> & getChunks();
	static ChunkCoord getChunkCoord(int x, int y, int z);
	void remove(Chunk* chunk);
private:
	void setAdjacentChunks(Chunk* chunk, const ChunkCoord& coord);

	std::unordered_map<ChunkCoord, Chunk*> modifiedChunks_;
	Generator* generator_;
};


