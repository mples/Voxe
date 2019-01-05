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
	World();
	~World();
	
	void setBlock(int x, int y, int z, BlockType type);
	void setChunk(int x, int y, int z, Chunk * chunk);
	Chunk* getChunk(ChunkCoord& coord);
	std::unordered_map<ChunkCoord, Chunk*> & getChunks();
	//TODO make method to activate and deactivate world in Graphic Engine
private:
	std::unordered_map<ChunkCoord, Chunk*> chunks_;
	
	ChunkCoord getChunkCoord(int x, int y, int z);
	void setAdjacentChunks(Chunk* chunk, ChunkCoord& coord);
	Generator generator_;

};


