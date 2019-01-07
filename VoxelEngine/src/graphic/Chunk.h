#pragma once
#include <glm/glm.hpp>

#include "BlockType.h"
#include "GraphicModel.h"

class Chunk {
public:
	using byte4 = glm::tvec4<GLbyte>;
	static const unsigned int CHUNK_DIM = 32;

	Chunk();
	~Chunk();

	void update();
	void inserXNegativeSide(std::vector<GLbyte> &vertices, std::vector<float> &texture_coord, const GLbyte &x, const GLbyte &y, const GLbyte &z, BlockType type);
	void insertXPositiveSide(std::vector<GLbyte>&vertices, std::vector<float> &texture_coord, const GLbyte &x, const GLbyte &y, const GLbyte &z, BlockType type);

	void insertYNegativeSide(std::vector<GLbyte> &vertices, std::vector<float> &texture_coord, const GLbyte &x, const GLbyte &y, const GLbyte &z, BlockType type);
	void insertYPositiveSide(std::vector<GLbyte>&vertices, std::vector<float> &texture_coord, const GLbyte &x, const GLbyte &y, const GLbyte &z, BlockType type);

	void insertZNegativeSide(std::vector<GLbyte> &vertices, std::vector<float> &texture_coord, const GLbyte &x, const GLbyte &y, const GLbyte &z, BlockType type);
	void insertZPositiveSide(std::vector<GLbyte>&vertices, std::vector<float> &texture_coord, const GLbyte &x, const GLbyte &y, const GLbyte &z, BlockType type);

	void draw();

	void setBlock(int x, int y, int z, BlockType type);
	BlockType getBlock(int x, int y, int z);
	Chunk* left_, *right_, *up_, *down_, *front_, *back_;
private:
	BlockType blocks_[CHUNK_DIM][CHUNK_DIM][CHUNK_DIM];
	bool changed_;
	GraphicModel model_;
	int elements_;

	bool isCovered(int x, int y, int z, int cov_x, int cov_y, int cov_z);
};

