#pragma once
#include "BlockType.h"
#include "Graphics/Mesh.h"

class Chunk {
public:
//	using byte4 = glm::tvec4<GLbyte>;
	static const unsigned int CHUNK_DIM = 32;

	Chunk();
	~Chunk();

	void update();

	void draw();

	void setBlock(int x, int y, int z, BlockType type);
	bool chagned();
	bool isEmpty();
	BlockType getBlock(int x, int y, int z);
	Chunk* left_, *right_, *up_, *down_, *front_, *back_;
private:
	void inserXNegativeSide(std::vector<float> &vertices, std::vector<float> &indices, std::vector<float> &texture_coord, const float &x, const float &y, const float &z, BlockType type);
	void insertXPositiveSide(std::vector<float>&vertices, std::vector<float> &indices, std::vector<float> &texture_coord, const float &x, const float &y, const float &z, BlockType type);

	void insertYNegativeSide(std::vector<float> &vertices, std::vector<float> &indices, std::vector<float> &texture_coord, const float &x, const float &y, const float &z, BlockType type);
	void insertYPositiveSide(std::vector<float>&vertices, std::vector<float> &indices, std::vector<float> &texture_coord, const float &x, const float &y, const float &z, BlockType type);

	void insertZNegativeSide(std::vector<float> &vertices, std::vector<float> &indices, std::vector<float> &texture_coord, const float &x, const float &y, const float &z, BlockType type);
	void insertZPositiveSide(std::vector<float>&vertices, std::vector<float> &indices, std::vector<float> &texture_coord, const float &x, const float &y, const float &z, BlockType type);

	BlockType blocks_[CHUNK_DIM][CHUNK_DIM][CHUNK_DIM];
	bool changed_;
	Mesh mesh_;
	int elements_;

	bool isCovered(int x, int y, int z, int cov_x, int cov_y, int cov_z);
};

