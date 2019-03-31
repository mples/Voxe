#pragma once
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Shader.h>
#include "Chunk.h"
#include "Generator.h"

using ChunkCoord = glm::ivec3;

template<>
class std::hash<ChunkCoord> {
public:
	std::size_t operator()(const ChunkCoord& coord) const {
		return std::hash<int>()(coord.x) ^ std::hash<int>()(coord.y) ^ std::hash<int>()(coord.z);
	}
};

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
	std::unordered_map<ChunkCoord, Chunk*> modifiedChunks_;
	Generator* generator_;
	
	void setAdjacentChunks(Chunk* chunk, const ChunkCoord& coord);

};

