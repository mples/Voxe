#include "Chunk.h"
#include "Camera.h"
#include "GraphicEngine.h"

Chunk::Chunk() : changed_(true){
	//memset(blocks_, 0, sizeof(blocks_));
	for (GLbyte x = 0; x < CHUNK_DIM; ++x) {
		for (GLbyte y = 0; y < CHUNK_DIM; ++y) {
			for (GLbyte z = 0; z < CHUNK_DIM; ++z) {
				blocks_[x][y][z] = BlockType::AIR;
			}
		}
	}
}


Chunk::~Chunk() {
}

void Chunk::update() {
	changed_ = false;

	std::vector<GLbyte> vertices; 
	std::vector<float> texture_coord;

	for (GLbyte x = 0; x < CHUNK_DIM; ++x) {
		for (GLbyte y = 0; y < CHUNK_DIM; ++y) {
			for (GLbyte z = 0; z < CHUNK_DIM; ++z) {
				BlockType type = blocks_[x][y][z];
				if (type == BlockType::AIR) {
					continue;
				}
				//TODO add checking on chunks egdes 
				if (x > 0 && blocks_[x - 1][y][z] == BlockType::AIR) {
					inserXNegativeSide(vertices, texture_coord, x, y, z);
				}
				if (x + 1 < CHUNK_DIM && blocks_[x + 1][y][z] == BlockType::AIR) {
					insertXPositiveSide(vertices, texture_coord, x, y, z);
				}
				if (y > 0 && blocks_[x][y - 1][z] == BlockType::AIR) {
					insertYNegativeSide(vertices, texture_coord, x, y, z);
				}
				if (y + 1 < CHUNK_DIM && blocks_[x][y + 1][z] == BlockType::AIR) {
					insertYPositiveSide(vertices, texture_coord, x, y, z);
				}
				if (z > 0 && blocks_[x][y][z - 1] == BlockType::AIR) {
					insertZNegativeSide(vertices, texture_coord, x, y, z);
				}
				if (z + 1 < CHUNK_DIM && blocks_[x][y][z + 1] == BlockType::AIR) {
					insertZPositiveSide(vertices, texture_coord, x, y, z);
				}
			}
		}
	}

	elements_ += vertices.size();
	model_.loadData(vertices, 3);
	model_.loadData(texture_coord, 2);
}

void Chunk::inserXNegativeSide(std::vector<GLbyte> &vertices, std::vector<float> &texture_coord, const GLbyte &x, const GLbyte &y, const GLbyte &z) {
	vertices.push_back(x);
	vertices.push_back(y);
	vertices.push_back(z);

	texture_coord.push_back(0);
	texture_coord.push_back(0);

	vertices.push_back(x);
	vertices.push_back(y);
	vertices.push_back(z + 1);

	texture_coord.push_back(1);
	texture_coord.push_back(0);

	vertices.push_back(x);
	vertices.push_back(y + 1);
	vertices.push_back(z);

	texture_coord.push_back(0);
	texture_coord.push_back(1);

	vertices.push_back(x);
	vertices.push_back(y + 1);
	vertices.push_back(z);

	texture_coord.push_back(0);
	texture_coord.push_back(1);

	vertices.push_back(x);
	vertices.push_back(y);
	vertices.push_back(z + 1);

	texture_coord.push_back(1);
	texture_coord.push_back(0);

	vertices.push_back(x);
	vertices.push_back(y + 1);
	vertices.push_back(z + 1);

	texture_coord.push_back(1);
	texture_coord.push_back(1);
}

void Chunk::insertXPositiveSide(std::vector<GLbyte> &vertices, std::vector<float> &texture_coord, const GLbyte &x, const GLbyte &y, const GLbyte &z) {
	vertices.push_back(x + 1);
	vertices.push_back(y);
	vertices.push_back(z);

	texture_coord.push_back(1);
	texture_coord.push_back(0);

	vertices.push_back(x + 1);
	vertices.push_back(y + 1);
	vertices.push_back(z);

	texture_coord.push_back(1);
	texture_coord.push_back(1);

	vertices.push_back(x + 1);
	vertices.push_back(y);
	vertices.push_back(z + 1);

	texture_coord.push_back(0);
	texture_coord.push_back(0);

	vertices.push_back(x + 1);
	vertices.push_back(y);
	vertices.push_back(z + 1);

	texture_coord.push_back(0);
	texture_coord.push_back(0);

	vertices.push_back(x + 1);
	vertices.push_back(y + 1);
	vertices.push_back(z);

	texture_coord.push_back(1);
	texture_coord.push_back(1);

	vertices.push_back(x + 1);
	vertices.push_back(y + 1);
	vertices.push_back(z + 1);

	texture_coord.push_back(0);
	texture_coord.push_back(1);

}

void Chunk::insertYNegativeSide(std::vector<GLbyte>& vertices, std::vector<float> &texture_coord, const GLbyte & x, const GLbyte & y, const GLbyte & z) {
	vertices.push_back(x);
	vertices.push_back(y);
	vertices.push_back(z);

	texture_coord.push_back(1);
	texture_coord.push_back(1);

	vertices.push_back(x + 1);
	vertices.push_back(y);
	vertices.push_back(z);

	texture_coord.push_back(0);
	texture_coord.push_back(1);

	vertices.push_back(x);
	vertices.push_back(y);
	vertices.push_back(z + 1);

	texture_coord.push_back(1);
	texture_coord.push_back(0);

	vertices.push_back(x);
	vertices.push_back(y);
	vertices.push_back(z + 1);

	texture_coord.push_back(1);
	texture_coord.push_back(0);

	vertices.push_back(x  + 1);
	vertices.push_back(y);
	vertices.push_back(z);

	texture_coord.push_back(0);
	texture_coord.push_back(1);

	vertices.push_back(x + 1);
	vertices.push_back(y);
	vertices.push_back(z + 1);

	texture_coord.push_back(0);
	texture_coord.push_back(0);

}

void Chunk::insertYPositiveSide(std::vector<GLbyte>& vertices, std::vector<float> &texture_coord, const GLbyte & x, const GLbyte & y, const GLbyte & z) {
	vertices.push_back(x);
	vertices.push_back(y + 1);
	vertices.push_back(z);

	texture_coord.push_back(0);
	texture_coord.push_back(1);

	vertices.push_back(x);
	vertices.push_back(y + 1);
	vertices.push_back(z + 1);

	texture_coord.push_back(0);
	texture_coord.push_back(0);

	vertices.push_back(x + 1);
	vertices.push_back(y + 1);
	vertices.push_back(z);

	texture_coord.push_back(1);
	texture_coord.push_back(1);

	vertices.push_back(x + 1);
	vertices.push_back(y + 1);
	vertices.push_back(z);

	texture_coord.push_back(1);
	texture_coord.push_back(1);

	vertices.push_back(x);
	vertices.push_back(y + 1);
	vertices.push_back(z + 1);

	texture_coord.push_back(0);
	texture_coord.push_back(0);

	vertices.push_back(x + 1);
	vertices.push_back(y + 1);
	vertices.push_back(z + 1);

	texture_coord.push_back(1);
	texture_coord.push_back(0);


}

void Chunk::insertZNegativeSide(std::vector<GLbyte>& vertices, std::vector<float> &texture_coord, const GLbyte & x, const GLbyte & y, const GLbyte & z) {
	vertices.push_back(x);
	vertices.push_back(y);
	vertices.push_back(z);

	texture_coord.push_back(0);
	texture_coord.push_back(0);

	vertices.push_back(x);
	vertices.push_back(y + 1);
	vertices.push_back(z);

	texture_coord.push_back(0);
	texture_coord.push_back(1);

	vertices.push_back(x + 1);
	vertices.push_back(y);
	vertices.push_back(z);

	texture_coord.push_back(1);
	texture_coord.push_back(0);

	vertices.push_back(x);
	vertices.push_back(y + 1);
	vertices.push_back(z);

	texture_coord.push_back(0);
	texture_coord.push_back(1);

	vertices.push_back(x + 1);
	vertices.push_back(y + 1);
	vertices.push_back(z);

	texture_coord.push_back(1);
	texture_coord.push_back(1);

	vertices.push_back(x + 1);
	vertices.push_back(y);
	vertices.push_back(z);

	texture_coord.push_back(1);
	texture_coord.push_back(0);

}

void Chunk::insertZPositiveSide(std::vector<GLbyte>& vertices, std::vector<float> &texture_coord, const GLbyte & x, const GLbyte & y, const GLbyte & z) {
	vertices.push_back(x);
	vertices.push_back(y);
	vertices.push_back(z + 1);

	texture_coord.push_back(0);
	texture_coord.push_back(0);

	vertices.push_back(x + 1);
	vertices.push_back(y);
	vertices.push_back(z + 1);

	texture_coord.push_back(1);
	texture_coord.push_back(0);

	vertices.push_back(x);
	vertices.push_back(y + 1);
	vertices.push_back(z + 1);

	texture_coord.push_back(0);
	texture_coord.push_back(1);

	vertices.push_back(x);
	vertices.push_back(y + 1);
	vertices.push_back(z + 1);

	texture_coord.push_back(0);
	texture_coord.push_back(1);

	vertices.push_back(x + 1);
	vertices.push_back(y);
	vertices.push_back(z + 1);

	texture_coord.push_back(1);
	texture_coord.push_back(0);

	vertices.push_back(x + 1);
	vertices.push_back(y + 1);
	vertices.push_back(z + 1);

	texture_coord.push_back(1);
	texture_coord.push_back(1);

}

void Chunk::draw() {
	if (changed_) {
		update();
	}
	if (!elements_)
		return;

	model_.bindVAO();
	glDrawArrays(GL_TRIANGLES, 0, elements_);

}

void Chunk::setBlock(int x, int y, int z, BlockType type) {
	blocks_[x][y][z] = type;
	changed_ = true;
}

BlockType Chunk::getBlock(int x, int y, int z) {
	return blocks_[x][y][z];
}
