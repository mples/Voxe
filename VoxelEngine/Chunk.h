#pragma once
#include <glm/glm.hpp>

#include "BlockType.h"
#include "GraphicModel.h"

class Chunk {
public:
	using byte4 = glm::tvec4<GLbyte>;
	static const unsigned int CHUNK_DIM = 16;

	Chunk();
	~Chunk();

	void update();
	void inserXNegativeSide(std::vector<GLbyte> &vertices, std::vector<float> &texture_coord, const GLbyte &x, const GLbyte &y, const GLbyte &z);
	void insertXPositiveSide(std::vector<GLbyte>&vertices, std::vector<float> &texture_coord, const GLbyte &x, const GLbyte &y, const GLbyte &z);

	void insertYNegativeSide(std::vector<GLbyte> &vertices, std::vector<float> &texture_coord, const GLbyte &x, const GLbyte &y, const GLbyte &z);
	void insertYPositiveSide(std::vector<GLbyte>&vertices, std::vector<float> &texture_coord, const GLbyte &x, const GLbyte &y, const GLbyte &z);

	void insertZNegativeSide(std::vector<GLbyte> &vertices, std::vector<float> &texture_coord, const GLbyte &x, const GLbyte &y, const GLbyte &z);
	void insertZPositiveSide(std::vector<GLbyte>&vertices, std::vector<float> &texture_coord, const GLbyte &x, const GLbyte &y, const GLbyte &z);

	void draw();

	void setBlock(int x, int y, int z, BlockType type);
	BlockType getBlock(int x, int y, int z);
private:
	BlockType blocks_[CHUNK_DIM][CHUNK_DIM][CHUNK_DIM];
	bool changed_;
	GraphicModel model_;
	int elements_;
};

