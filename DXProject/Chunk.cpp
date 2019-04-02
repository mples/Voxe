#include "Chunk.h"
//
//Chunk::Chunk() : changed_(true), left_(nullptr), right_(nullptr), up_(nullptr), down_(nullptr), front_(nullptr), back_(nullptr) {
//	//memset(blocks_, 0, sizeof(blocks_));
//	for (BYTE x = 0; x < CHUNK_DIM; ++x) {
//		for (BYTE y = 0; y < CHUNK_DIM; ++y) {
//			for (BYTE z = 0; z < CHUNK_DIM; ++z) {
//				blocks_[x][y][z] = BlockType::AIR;
//			}
//		}
//	}
//}
//
//
//Chunk::~Chunk() {
//}
//
//void Chunk::update() {
//	changed_ = false;
//	model_.clearData();
//	std::vector<float> vertices;
//	std::vector<DWORD> indices;
//	std::vector<float> texture_coord;
//
//	for (UINT x = 0; x < CHUNK_DIM; ++x) {
//		for (UINT y = 0; y < CHUNK_DIM; ++y) {
//			for (UINT z = 0; z < CHUNK_DIM; ++z) {
//				BlockType type = blocks_[x][y][z];
//				if (type == BlockType::AIR) {
//					continue;
//				}
//				if (x > 0 && blocks_[x - 1][y][z] == BlockType::AIR) {
//					inserXNegativeSide(vertices, texture_coord, x, y, z, type);
//				}
//				else if (x == 0) {
//					if (!left_ || (left_->getBlock(CHUNK_DIM - 1, y, z)) == BlockType::AIR ) {
//						inserXNegativeSide(vertices, texture_coord, x, y, z, type);
//					}
//				}
//				if (x + 1 < CHUNK_DIM && blocks_[x + 1][y][z] == BlockType::AIR) {
//					insertXPositiveSide(vertices, texture_coord, x, y, z, type);
//				}
//				else if (x + 1 == CHUNK_DIM) {
//					if (!right_|| (right_->getBlock(0, y, z)) == BlockType::AIR) {
//						insertXPositiveSide(vertices, texture_coord, x, y, z, type);
//					}
//				}
//				if (y > 0 && blocks_[x][y - 1][z] == BlockType::AIR) {
//					insertYNegativeSide(vertices, texture_coord, x, y, z, type);
//				}
//				else if (y == 0) {
//					if (!down_ || (down_->getBlock(x, CHUNK_DIM - 1, z)) == BlockType::AIR) {
//						insertYNegativeSide(vertices, texture_coord, x, y, z, type);
//					}
//				}
//				if (y + 1 < CHUNK_DIM && blocks_[x][y + 1][z] == BlockType::AIR) {
//					insertYPositiveSide(vertices, texture_coord, x, y, z, type);
//				}
//				else if (y  == CHUNK_DIM - 1) {
//					if (!up_|| (up_->getBlock(x, 0, z)) == BlockType::AIR) {
//						insertYPositiveSide(vertices, texture_coord, x, y, z, type);
//					}
//				}
//				if (z > 0 && blocks_[x][y][z - 1] == BlockType::AIR) {
//					insertZNegativeSide(vertices, texture_coord, x, y, z, type);
//				}
//				else if (z == 0) {
//					if (!front_ || (front_->getBlock(x, y, CHUNK_DIM - 1)) == BlockType::AIR) {
//						insertZNegativeSide(vertices, texture_coord, x, y, z, type);
//					}
//				}
//				if (z + 1 < CHUNK_DIM && blocks_[x][y][z + 1] == BlockType::AIR) {
//					insertZPositiveSide(vertices, texture_coord, x, y, z, type);
//				}
//				else if (z + 1 == CHUNK_DIM) {
//					if (!back_ || (back_->getBlock(x, y, 0)) == BlockType::AIR) {
//						insertZPositiveSide(vertices, texture_coord, x, y, z, type);
//					}
//				}
//			}
//		}
//	}
//
//	elements_ = vertices.size();
//	model_.loadData(vertices, 3);
//	model_.loadData(texture_coord, 2);
//}
//
//void Chunk::inserXNegativeSide(std::vector<float> &vertices, std::vector<float> &texture_coord, const float &x, const float &y, const float &z, BlockType type) {
//	std::unordered_map<TextureVertex, glm::vec2> coord = BlockManager::getInstance().getTextureAtlas()->getTextureCoord(type, TextureSide::LEFT);
//	vertices.push_back(x);
//	vertices.push_back(y);
//	vertices.push_back(z);
//
//	texture_coord.push_back(coord.at(TextureVertex::_00).x);
//	texture_coord.push_back(coord.at(TextureVertex::_00).y);
//
//	vertices.push_back(x);
//	vertices.push_back(y);
//	vertices.push_back(z + 1);
//
//	texture_coord.push_back(coord.at(TextureVertex::_10).x);
//	texture_coord.push_back(coord.at(TextureVertex::_10).y);
//
//	vertices.push_back(x);
//	vertices.push_back(y + 1);
//	vertices.push_back(z);
//
//	texture_coord.push_back(coord.at(TextureVertex::_01).x);
//	texture_coord.push_back(coord.at(TextureVertex::_01).y);
//
//	vertices.push_back(x);
//	vertices.push_back(y + 1);
//	vertices.push_back(z);
//
//	texture_coord.push_back(coord.at(TextureVertex::_01).x);
//	texture_coord.push_back(coord.at(TextureVertex::_01).y);
//
//	vertices.push_back(x);
//	vertices.push_back(y);
//	vertices.push_back(z + 1);
//
//	texture_coord.push_back(coord.at(TextureVertex::_10).x);
//	texture_coord.push_back(coord.at(TextureVertex::_10).y);
//
//	vertices.push_back(x);
//	vertices.push_back(y + 1);
//	vertices.push_back(z + 1);
//
//	texture_coord.push_back(coord.at(TextureVertex::_11).x);
//	texture_coord.push_back(coord.at(TextureVertex::_11).y);
//}
//
//void Chunk::insertXPositiveSide(std::vector<float> &vertices, std::vector<float> &texture_coord, const float &x, const float &y, const float &z, BlockType type) {
//	std::unordered_map<TextureVertex, glm::vec2> coord = BlockManager::getInstance().getTextureAtlas()->getTextureCoord(type, TextureSide::RIGHT);
//
//	vertices.push_back(x + 1);
//	vertices.push_back(y);
//	vertices.push_back(z);
//	//texture_coord.push_back(1);
//	//texture_coord.push_back(0);
//	texture_coord.push_back(coord.at(TextureVertex::_10).x);
//	texture_coord.push_back(coord.at(TextureVertex::_10).y);
//
//	vertices.push_back(x + 1);
//	vertices.push_back(y + 1);
//	vertices.push_back(z);
//	//texture_coord.push_back(1);
//	//texture_coord.push_back(1);
//	texture_coord.push_back(coord.at(TextureVertex::_11).x);
//	texture_coord.push_back(coord.at(TextureVertex::_11).y);
//
//	vertices.push_back(x + 1);
//	vertices.push_back(y);
//	vertices.push_back(z + 1);
//	//texture_coord.push_back(0);
//	//texture_coord.push_back(0);
//	texture_coord.push_back(coord.at(TextureVertex::_00).x);
//	texture_coord.push_back(coord.at(TextureVertex::_00).y);
//
//	vertices.push_back(x + 1);
//	vertices.push_back(y);
//	vertices.push_back(z + 1);
//	//texture_coord.push_back(0);
//	//texture_coord.push_back(0);
//	texture_coord.push_back(coord.at(TextureVertex::_00).x);
//	texture_coord.push_back(coord.at(TextureVertex::_00).y);
//
//	vertices.push_back(x + 1);
//	vertices.push_back(y + 1);
//	vertices.push_back(z);
//	//texture_coord.push_back(1);
//	//texture_coord.push_back(1);
//	texture_coord.push_back(coord.at(TextureVertex::_11).x);
//	texture_coord.push_back(coord.at(TextureVertex::_11).y);
//
//	vertices.push_back(x + 1);
//	vertices.push_back(y + 1);
//	vertices.push_back(z + 1);
//	//texture_coord.push_back(0);
//	//texture_coord.push_back(1);
//	texture_coord.push_back(coord.at(TextureVertex::_01).x);
//	texture_coord.push_back(coord.at(TextureVertex::_01).y);
//
//}
//
//void Chunk::insertYNegativeSide(std::vector<float>& vertices, std::vector<float> &texture_coord, const float & x, const float & y, const float & z, BlockType type) {
//	std::unordered_map<TextureVertex, glm::vec2> coord = BlockManager::getInstance().getTextureAtlas()->getTextureCoord(type, TextureSide::BOTTOM);
//
//	vertices.push_back(x);
//	vertices.push_back(y);
//	vertices.push_back(z);
//
//	texture_coord.push_back(coord.at(TextureVertex::_11).x);
//	texture_coord.push_back(coord.at(TextureVertex::_11).y);
//
//	vertices.push_back(x + 1);
//	vertices.push_back(y);
//	vertices.push_back(z);
//
//	texture_coord.push_back(coord.at(TextureVertex::_01).x);
//	texture_coord.push_back(coord.at(TextureVertex::_01).y);
//
//	vertices.push_back(x);
//	vertices.push_back(y);
//	vertices.push_back(z + 1);
//
//	texture_coord.push_back(coord.at(TextureVertex::_10).x);
//	texture_coord.push_back(coord.at(TextureVertex::_10).y);
//
//	vertices.push_back(x);
//	vertices.push_back(y);
//	vertices.push_back(z + 1);
//
//	texture_coord.push_back(coord.at(TextureVertex::_10).x);
//	texture_coord.push_back(coord.at(TextureVertex::_10).y);
//
//	vertices.push_back(x  + 1);
//	vertices.push_back(y);
//	vertices.push_back(z);
//
//	texture_coord.push_back(coord.at(TextureVertex::_01).x);
//	texture_coord.push_back(coord.at(TextureVertex::_01).y);
//
//	vertices.push_back(x + 1);
//	vertices.push_back(y);
//	vertices.push_back(z + 1);
//
//	texture_coord.push_back(coord.at(TextureVertex::_00).x);
//	texture_coord.push_back(coord.at(TextureVertex::_00).y);
//
//}
//
//void Chunk::insertYPositiveSide(std::vector<float>& vertices, std::vector<float> &texture_coord, const float & x, const float & y, const float & z, BlockType type) {
//	std::unordered_map<TextureVertex, glm::vec2> coord = BlockManager::getInstance().getTextureAtlas()->getTextureCoord(type, TextureSide::TOP);
//
//	vertices.push_back(x);
//	vertices.push_back(y + 1);
//	vertices.push_back(z);
//
//	texture_coord.push_back(coord.at(TextureVertex::_01).x);
//	texture_coord.push_back(coord.at(TextureVertex::_01).y);
//
//	vertices.push_back(x);
//	vertices.push_back(y + 1);
//	vertices.push_back(z + 1);
//
//	texture_coord.push_back(coord.at(TextureVertex::_00).x);
//	texture_coord.push_back(coord.at(TextureVertex::_00).y);
//
//
//	vertices.push_back(x + 1);
//	vertices.push_back(y + 1);
//	vertices.push_back(z);
//
//	texture_coord.push_back(coord.at(TextureVertex::_11).x);
//	texture_coord.push_back(coord.at(TextureVertex::_11).y);
//
//	vertices.push_back(x + 1);
//	vertices.push_back(y + 1);
//	vertices.push_back(z);
//
//	texture_coord.push_back(coord.at(TextureVertex::_11).x);
//	texture_coord.push_back(coord.at(TextureVertex::_11).y);
//
//	vertices.push_back(x);
//	vertices.push_back(y + 1);
//	vertices.push_back(z + 1);
//
//	texture_coord.push_back(coord.at(TextureVertex::_00).x);
//	texture_coord.push_back(coord.at(TextureVertex::_00).y);
//
//	vertices.push_back(x + 1);
//	vertices.push_back(y + 1);
//	vertices.push_back(z + 1);
//
//	texture_coord.push_back(coord.at(TextureVertex::_10).x);
//	texture_coord.push_back(coord.at(TextureVertex::_10).y);
//
//
//}
//
//void Chunk::insertZNegativeSide(std::vector<float>& vertices, std::vector<float> &texture_coord, const float & x, const float & y, const float & z, BlockType type) {
//	std::unordered_map<TextureVertex, glm::vec2> coord = BlockManager::getInstance().getTextureAtlas()->getTextureCoord(type, TextureSide::FRONT);
//
//	vertices.push_back(x);
//	vertices.push_back(y);
//	vertices.push_back(z);
//
//	texture_coord.push_back(coord.at(TextureVertex::_10).x);
//	texture_coord.push_back(coord.at(TextureVertex::_10).y);
//
//	vertices.push_back(x);
//	vertices.push_back(y + 1);
//	vertices.push_back(z);
//
//	texture_coord.push_back(coord.at(TextureVertex::_11).x);
//	texture_coord.push_back(coord.at(TextureVertex::_11).y);
//
//	vertices.push_back(x + 1);
//	vertices.push_back(y);
//	vertices.push_back(z);
//
//	texture_coord.push_back(coord.at(TextureVertex::_00).x);
//	texture_coord.push_back(coord.at(TextureVertex::_00).y);
//
//	vertices.push_back(x);
//	vertices.push_back(y + 1);
//	vertices.push_back(z);
//
//	texture_coord.push_back(coord.at(TextureVertex::_11).x);
//	texture_coord.push_back(coord.at(TextureVertex::_11).y);
//
//	vertices.push_back(x + 1);
//	vertices.push_back(y + 1);
//	vertices.push_back(z);
//
//	texture_coord.push_back(coord.at(TextureVertex::_01).x);
//	texture_coord.push_back(coord.at(TextureVertex::_01).y);
//
//	vertices.push_back(x + 1);
//	vertices.push_back(y);
//	vertices.push_back(z);
//
//	texture_coord.push_back(coord.at(TextureVertex::_00).x);
//	texture_coord.push_back(coord.at(TextureVertex::_00).y);
//
//}
//
//void Chunk::insertZPositiveSide(std::vector<float>& vertices, std::vector<float> &texture_coord, const float & x, const float & y, const float & z, BlockType type) {
//	std::unordered_map<TextureVertex, glm::vec2> coord = BlockManager::getInstance().getTextureAtlas()->getTextureCoord(type, TextureSide::BACK);
//	vertices.push_back(x);
//	vertices.push_back(y);
//	vertices.push_back(z + 1);
//
//	texture_coord.push_back(coord.at(TextureVertex::_00).x);
//	texture_coord.push_back(coord.at(TextureVertex::_00).y);
//	//texture_coord.push_back(0);
//	//texture_coord.push_back(0);
//
//	vertices.push_back(x + 1);
//	vertices.push_back(y);
//	vertices.push_back(z + 1);
//
//	texture_coord.push_back(coord.at(TextureVertex::_10).x);
//	texture_coord.push_back(coord.at(TextureVertex::_10).y);
//	//texture_coord.push_back(1);
//	//texture_coord.push_back(0);
//	vertices.push_back(x);
//	vertices.push_back(y + 1);
//	vertices.push_back(z + 1);
//
//	texture_coord.push_back(coord.at(TextureVertex::_01).x);
//	texture_coord.push_back(coord.at(TextureVertex::_01).y);
//	//texture_coord.push_back(0);
//	//texture_coord.push_back(1);
//	vertices.push_back(x);
//	vertices.push_back(y + 1);
//	vertices.push_back(z + 1);
//
//	texture_coord.push_back(coord.at(TextureVertex::_01).x);
//	texture_coord.push_back(coord.at(TextureVertex::_01).y);
//	//texture_coord.push_back(0);
//	//texture_coord.push_back(1);
//	vertices.push_back(x + 1);
//	vertices.push_back(y);
//	vertices.push_back(z + 1);
//
//	texture_coord.push_back(coord.at(TextureVertex::_10).x);
//	texture_coord.push_back(coord.at(TextureVertex::_10).y);
//	//texture_coord.push_back(1);
//	//texture_coord.push_back(0);
//	vertices.push_back(x + 1);
//	vertices.push_back(y + 1);
//	vertices.push_back(z + 1);
//	//texture_coord.push_back(1);
//	//texture_coord.push_back(1);
//	texture_coord.push_back(coord.at(TextureVertex::_11).x);
//	texture_coord.push_back(coord.at(TextureVertex::_11).y);
//
//}
//
//void Chunk::draw() {
//	/*if (changed_) {
//		update();
//	}*/
//	if (!elements_)
//		return;
//
//	model_.bindVAO();
//	glDrawArrays(GL_TRIANGLES, 0, elements_);
//
//}
//
//void Chunk::setBlock(int x, int y, int z, BlockType type) {
//	//std::cout << "Chunk x: " << x << " y: " << y << " z: " << z << std::endl;
//	if (x < 0 || x >= CHUNK_DIM || y < 0 || y >= CHUNK_DIM || z < 0 || z >= CHUNK_DIM)
//		return;
//	blocks_[x][y][z] = type;
//	changed_ = true;
//
//	if (x == 0 && left_ != nullptr) {
//		left_->changed_ = true;
//	}else if (x == CHUNK_DIM - 1 && right_ != nullptr) {
//		right_->changed_ = true;
//	}
//	if (y == CHUNK_DIM - 1 && up_ != nullptr) {
//		up_->changed_ = true;
//	}
//	else if (y == 0 && down_ != nullptr) {
//		down_->changed_ = true;
//	}
//	if (z == 0 && front_ != nullptr) {
//		front_->changed_ = true;
//	}
//	else if (z == CHUNK_DIM - 1 && back_ != nullptr) {
//		back_->changed_ = true;
//	}
//}
//
//bool Chunk::chagned() {
//	return changed_;
//}
//
//bool Chunk::isEmpty() {
//	return elements_ == 0 ;
//}
//
//BlockType Chunk::getBlock(int x, int y, int z) {
//	return blocks_[x][y][z];
//}
//
//bool Chunk::isCovered(int x, int y, int z, int cov_x, int cov_y, int cov_z) {
//	//TODO clean method or make it usefull
//	if (blocks_[cov_x][cov_y][cov_z] != BlockType::AIR) {
//		return true;
//	}
//
//	return false;
//}
