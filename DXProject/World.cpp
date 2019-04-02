#include "World.h"
#include <cmath>

//
//World::World(Generator* gen) : generator_(gen) {
//	GraphicEngine::getInstance().setWorld(this);
//}
//
//
//World::~World() {
//	if (generator_ != nullptr)
//		delete generator_;
//}
//
//
//void World::setBlock(int x, int y, int z, BlockType type) {
//
//	ChunkCoord coord = getChunkCoord(x, y, z);
//	auto found = modifiedChunks_.find(coord);
//	if (found == modifiedChunks_.end()) {
//		Chunk* new_chunk = getChunk(x, y, z);
//		new_chunk->setBlock(x % Chunk::CHUNK_DIM, y % Chunk::CHUNK_DIM, z % Chunk::CHUNK_DIM, type);
//		return;
//	}
//	found->second->setBlock(x % Chunk::CHUNK_DIM, y % Chunk::CHUNK_DIM, z % Chunk::CHUNK_DIM, type);
//}
//
//void World::setChunk(int x, int y, int z, Chunk * chunk) {
//	ChunkCoord coord(x, y, z);
//	auto found = modifiedChunks_.find(coord);
//	if (found == modifiedChunks_.end()) {
//		modifiedChunks_.insert(std::make_pair(coord, chunk));
//		setAdjacentChunks(chunk, coord);
//		return;
//	}
//}
//
//std::unordered_map<ChunkCoord, Chunk*>& World::getChunks() {
//	return modifiedChunks_;
//}
//
//ChunkCoord World::getChunkCoord(int x, int y, int z) {
//	return ChunkCoord(std::floor((double)x / Chunk::CHUNK_DIM) , std::floor((double)y / Chunk::CHUNK_DIM), std::floor((double)z / Chunk::CHUNK_DIM));
//}
//
//void World::setAdjacentChunks(Chunk * chunk, const ChunkCoord& coord) {
//	ChunkCoord left_coord = ChunkCoord(coord.x -1 , coord.y, coord.z);
//	auto left_chunk = modifiedChunks_.find(left_coord);
//	if (left_chunk != modifiedChunks_.end()) {
//		chunk->left_ = left_chunk->second;
//		left_chunk->second->right_ = chunk;
//	}
//
//	ChunkCoord right_coord = ChunkCoord(coord.x + 1, coord.y, coord.z);
//	auto right_chunk = modifiedChunks_.find(right_coord);
//	if (right_chunk != modifiedChunks_.end()) {
//		chunk->right_ = right_chunk->second;
//		right_chunk->second->left_= chunk;
//	}
//
//	ChunkCoord up_coord = ChunkCoord(coord.x, coord.y + 1, coord.z);
//	auto up_chunk = modifiedChunks_.find(up_coord);
//	if (up_chunk != modifiedChunks_.end()) {
//		chunk->up_ = up_chunk->second;
//		up_chunk->second->down_ = chunk;
//	}
//
//	ChunkCoord down_coord = ChunkCoord(coord.x, coord.y - 1, coord.z);
//	auto down_chunk = modifiedChunks_.find(down_coord);
//	if (down_chunk != modifiedChunks_.end()) {
//		chunk->down_= down_chunk->second;
//		down_chunk->second->up_ = chunk;
//	}
//
//	ChunkCoord front_coord = ChunkCoord(coord.x, coord.y, coord.z  - 1);
//	auto front_chunk = modifiedChunks_.find(front_coord);
//	if (front_chunk != modifiedChunks_.end()) {
//		chunk->front_= front_chunk->second;
//		front_chunk->second->back_ = chunk;
//	}
//
//	ChunkCoord back_coord = ChunkCoord(coord.x, coord.y, coord.z + 1);
//	auto back_chunk = modifiedChunks_.find(back_coord);
//	if (back_chunk != modifiedChunks_.end()) {
//		chunk->back_ = back_chunk->second;
//		back_chunk->second->front_ = chunk;
//	}
//
//}
//
//Chunk * World::getChunk(ChunkCoord& coord) {
//	auto found = modifiedChunks_.find(coord);
//	if (found != modifiedChunks_.end()) {
//		return found->second;
//	}
//	
//	Chunk* chunk = generator_->generate(coord);
//	setAdjacentChunks(chunk, coord);
//	modifiedChunks_.insert(std::make_pair(coord, chunk));
//	return chunk;
//}
//
//Chunk * World::getChunk(int x, int y, int z) {
//	ChunkCoord coord(x, y, z);
//	return getChunk(coord);
//}
