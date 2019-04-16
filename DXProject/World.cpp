#include "World.h"
#include <cmath>


World::World(Generator* gen) : generator_(gen) {
	
}


World::~World() {
//	if (generator_ != nullptr)
//		delete generator_;
}


void World::setBlock(int x, int y, int z, BlockType type) {

	ChunkCoord coord = getChunkCoord(x, y, z);
	auto found = modifiedChunks_.find(coord);
	if (found == modifiedChunks_.end()) {
		Chunk* new_chunk = getChunk(x, y, z);
		new_chunk->setBlock(x % Chunk::DIM, y % Chunk::DIM, z % Chunk::DIM, type);
		return;
	}
	found->second->setBlock(x % Chunk::DIM, y % Chunk::DIM, z % Chunk::DIM, type);
}

void World::setChunk(int x, int y, int z, Chunk * chunk) {
	ChunkCoord coord(x, y, z);
	auto found = modifiedChunks_.find(coord);
	if (found == modifiedChunks_.end()) {
		modifiedChunks_.insert(std::make_pair(coord, chunk));
		setAdjacentChunks(chunk, coord);
		return;
	}
}

std::unordered_map<ChunkCoord, Chunk*>& World::getChunks() {
	return modifiedChunks_;
}

ChunkCoord World::getChunkCoord(int x, int y, int z) {
	return ChunkCoord(std::floor((double)x / Chunk::DIM) , std::floor((double)y / Chunk::DIM), std::floor((double)z / Chunk::DIM));
}

void World::remove(Chunk * chunk) {
	auto found = modifiedChunks_.find(chunk->getCoord());
	if (found != modifiedChunks_.end()) {
		modifiedChunks_.erase(found);
	}
}

void World::setAdjacentChunks(Chunk * chunk, const ChunkCoord& coord) {
	ChunkCoord left_coord = ChunkCoord(coord.x_ - 1 , coord.y_, coord.z_);
	auto left_chunk = modifiedChunks_.find(left_coord);
	if (left_chunk != modifiedChunks_.end()) {
		chunk->neighbours_.left_ = left_chunk->second;
		left_chunk->second->neighbours_.right_ = chunk;
		left_chunk->second->changed_ = true;
	}

	ChunkCoord right_coord = ChunkCoord(coord.x_ + 1, coord.y_, coord.z_);
	auto right_chunk = modifiedChunks_.find(right_coord);
	if (right_chunk != modifiedChunks_.end()) {
		chunk->neighbours_.right_ = right_chunk->second;
		right_chunk->second->neighbours_.left_= chunk;
		right_chunk->second->changed_ = true;
	}

	ChunkCoord up_coord = ChunkCoord(coord.x_, coord.y_ + 1, coord.z_);
	auto up_chunk = modifiedChunks_.find(up_coord);
	if (up_chunk != modifiedChunks_.end()) {
		chunk->neighbours_.up_ = up_chunk->second;
		up_chunk->second->neighbours_.down_ = chunk;
		up_chunk->second->changed_ = true;
	}

	ChunkCoord down_coord = ChunkCoord(coord.x_, coord.y_ - 1, coord.z_);
	auto down_chunk = modifiedChunks_.find(down_coord);
	if (down_chunk != modifiedChunks_.end()) {
		chunk->neighbours_.down_= down_chunk->second;
		down_chunk->second->neighbours_.up_ = chunk;
		down_chunk->second->changed_ = true;
	}

	ChunkCoord front_coord = ChunkCoord(coord.x_, coord.y_, coord.z_  + 1);
	auto front_chunk = modifiedChunks_.find(front_coord);
	if (front_chunk != modifiedChunks_.end()) {
		chunk->neighbours_.front_= front_chunk->second;
		front_chunk->second->neighbours_.back_ = chunk;
		front_chunk->second->changed_ = true;
	}

	ChunkCoord back_coord = ChunkCoord(coord.x_, coord.y_, coord.z_ - 1);
	auto back_chunk = modifiedChunks_.find(back_coord);
	if (back_chunk != modifiedChunks_.end()) {
		chunk->neighbours_.back_ = back_chunk->second;
		back_chunk->second->neighbours_.front_ = chunk;
		back_chunk->second->changed_ = true;
	}

}

Chunk * World::getChunk(ChunkCoord& coord) {
	auto found = modifiedChunks_.find(coord);
	if (found != modifiedChunks_.end()) {
		return found->second;
	}
	
	BlockType blocks[Chunk::DIM][Chunk::DIM][Chunk::DIM] = {};
	generator_->generate(blocks, coord);
	Chunk* chunk = new Chunk(coord, blocks);
	setAdjacentChunks(chunk, coord);
	modifiedChunks_.insert(std::make_pair(coord, chunk));
	return chunk;
}

Chunk * World::getChunk(int x, int y, int z) {
	ChunkCoord coord(x, y, z);
	return getChunk(coord);
}
