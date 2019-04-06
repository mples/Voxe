#include "Chunk.h"

Chunk::Chunk() : changed_(true), left_(nullptr), right_(nullptr), up_(nullptr), down_(nullptr), front_(nullptr), back_(nullptr) {
	ZeroMemory(blocks_, pow(DIM, 3) * sizeof(BlockType));
}


Chunk::~Chunk() {
}

bool Chunk::initialize(ID3D11Device * device, ID3D11DeviceContext * device_context, ConstantBuffer<CB_VS_object_buffer>& cb_vertex_shader, Texture * tex) {
	/*for (BYTE x = 0; x < DIM; ++x) {
		for (BYTE y = 0; y < DIM; ++y) {
			for (BYTE z = 0; z < DIM; ++z) {
				blocks_[x][y][z] = BlockType::GRASS;
			}
		}
	}*/
	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;

	for (UINT x = 0; x < DIM; ++x) {
		for (UINT y = 0; y < DIM; ++y) {
			for (UINT z = 0; z < DIM; ++z) {
				BlockType type = blocks_[x][y][z];
				if (type != BlockType::AIR) {
					calculateVertices(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), type, vertices, indices);
				}
			}
		}
	}

	elements_ = vertices.size();
	if (elements_ > 0) {
		boundingBox_.CreateFromPoints(boundingBox_, vertices.size(), &(vertices.data()->pos_), sizeof(Vertex));
		model_.initialize(device, device_context, cb_vertex_shader, vertices, indices, tex);
		update();

	}
	return true;
}

void Chunk::update() {

	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;

	for (UINT x = 0; x < DIM; ++x) {
		for (UINT y = 0; y < DIM; ++y) {
			for (UINT z = 0; z < DIM; ++z) {
				BlockType type = blocks_[x][y][z];
				if (type != BlockType::AIR) {
					calculateVertices(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), type, vertices, indices);
				}
			}
		}
	}

	elements_ = vertices.size();
	if (elements_ > 0) {
		boundingBox_.CreateFromPoints(boundingBox_, vertices.size(), &(vertices.data()->pos_), sizeof(Vertex));
		model_.loadData(vertices, indices);
	}
	
	changed_ = false;
}

void Chunk::draw(XMMATRIX model_matrix, XMMATRIX view_proj_matrix) {
	if (changed_) {
		update();
	}
	if (!elements_)
		return;

	model_.draw(model_matrix, view_proj_matrix);

}

void Chunk::setBlock(int x, int y, int z, BlockType type) {
	//std::cout << "Chunk x: " << x << " y: " << y << " z: " << z << std::endl;
	if (x < 0 || x >= DIM || y < 0 || y >= DIM || z < 0 || z >= DIM)
		return;
	blocks_[x][y][z] = type;
	changed_ = true;

	if (x == 0 && left_ != nullptr) {
		left_->changed_ = true;
	}else if (x == DIM - 1 && right_ != nullptr) {
		right_->changed_ = true;
	}
	if (y == DIM - 1 && up_ != nullptr) {
		up_->changed_ = true;
	}
	else if (y == 0 && down_ != nullptr) {
		down_->changed_ = true;
	}
	if (z == 0 && front_ != nullptr) {
		front_->changed_ = true;
	}
	else if (z == DIM - 1 && back_ != nullptr) {
		back_->changed_ = true;
	}
}

bool Chunk::chagned() {
	return changed_;
}

bool Chunk::isEmpty() {
	return elements_ == 0 ;
}

BlockType Chunk::getBlock(int x, int y, int z) {
	return blocks_[x][y][z];
}

BoundingBox & Chunk::getBoundingBox() {
	return boundingBox_;
}

void Chunk::insertNegativeX(float x, float y, float z, BlockType type, std::vector<Vertex>& vertices, std::vector<DWORD>& indices) {
	UINT last_index = vertices.size();
	vertices.push_back(Vertex(XMFLOAT3(x, y, z), XMFLOAT2(1.0f, 1.0f), NEG_X_NORMAL));
	vertices.push_back(Vertex(XMFLOAT3(x, y, z + 1), XMFLOAT2(0.0f, 1.0f), NEG_X_NORMAL));
	vertices.push_back(Vertex(XMFLOAT3(x, y + 1, z), XMFLOAT2(1.0f, 0.0f), NEG_X_NORMAL));
	vertices.push_back(Vertex(XMFLOAT3(x, y + 1, z + 1), XMFLOAT2(0.0f, 0.0f), NEG_X_NORMAL));

	indices.push_back(last_index + 0);
	indices.push_back(last_index + 1);
	indices.push_back(last_index + 2);

	indices.push_back(last_index + 1);
	indices.push_back(last_index + 3);
	indices.push_back(last_index + 2);
}

void Chunk::insertPositiveX(float x, float y, float z, BlockType type, std::vector<Vertex>& vertices, std::vector<DWORD>& indices) {
	UINT last_index = vertices.size();
	vertices.push_back(Vertex(XMFLOAT3(x + 1, y, z), XMFLOAT2(0.0f, 1.0f), POS_X_NORMAL));
	vertices.push_back(Vertex(XMFLOAT3(x + 1, y, z + 1), XMFLOAT2(1.0f, 1.0f), POS_X_NORMAL));
	vertices.push_back(Vertex(XMFLOAT3(x + 1, y + 1, z + 1), XMFLOAT2(1.0f, 0.0f), POS_X_NORMAL));
	vertices.push_back(Vertex(XMFLOAT3(x + 1, y + 1, z), XMFLOAT2(0.0f, 0.0f), POS_X_NORMAL));

	indices.push_back(last_index + 0);
	indices.push_back(last_index + 2);
	indices.push_back(last_index + 1);

	indices.push_back(last_index + 0);
	indices.push_back(last_index + 3);
	indices.push_back(last_index + 2);
}

void Chunk::insertNegativeY(float x, float y, float z, BlockType type, std::vector<Vertex>& vertices, std::vector<DWORD>& indices) {
	UINT last_index = vertices.size();
	vertices.push_back(Vertex(XMFLOAT3(x, y, z), XMFLOAT2(1.0f, 1.0f), NEG_Y_NORMAL));
	vertices.push_back(Vertex(XMFLOAT3(x + 1, y, z), XMFLOAT2(0.0f, 1.0f), NEG_Y_NORMAL));
	vertices.push_back(Vertex(XMFLOAT3(x + 1, y, z + 1), XMFLOAT2(0.0f, 0.0f), NEG_Y_NORMAL));
	vertices.push_back(Vertex(XMFLOAT3(x, y, z + 1), XMFLOAT2(1.0f, 0.0f), NEG_Y_NORMAL));

	indices.push_back(last_index + 0);
	indices.push_back(last_index + 1);
	indices.push_back(last_index + 2);

	indices.push_back(last_index + 0);
	indices.push_back(last_index + 2);
	indices.push_back(last_index + 3);
}

void Chunk::insertPositiveY(float x, float y, float z, BlockType type, std::vector<Vertex>& vertices, std::vector<DWORD>& indices) {
	UINT last_index = vertices.size();
	vertices.push_back(Vertex(XMFLOAT3(x, y + 1, z), XMFLOAT2(0.0f, 1.0f), POS_Y_NORMAL));
	vertices.push_back(Vertex(XMFLOAT3(x + 1, y + 1, z), XMFLOAT2(1.0f, 1.0f), POS_Y_NORMAL));
	vertices.push_back(Vertex(XMFLOAT3(x + 1, y + 1, z + 1), XMFLOAT2(1.0f, 0.0f), POS_Y_NORMAL));
	vertices.push_back(Vertex(XMFLOAT3(x, y + 1, z + 1), XMFLOAT2(0.0f, 0.0f), POS_Y_NORMAL));

	indices.push_back(last_index + 0);
	indices.push_back(last_index + 2);
	indices.push_back(last_index + 1);

	indices.push_back(last_index + 0);
	indices.push_back(last_index + 3);
	indices.push_back(last_index + 2);
}

void Chunk::insertNegativeZ(float x, float y, float z, BlockType type, std::vector<Vertex>& vertices, std::vector<DWORD>& indices) {
	UINT last_index = vertices.size();
	vertices.push_back(Vertex(XMFLOAT3(x, y, z), XMFLOAT2(0.0f, 1.0f), NEG_Z_NORMAL));
	vertices.push_back(Vertex(XMFLOAT3(x + 1, y, z), XMFLOAT2(1.0f, 1.0f), NEG_Z_NORMAL));
	vertices.push_back(Vertex(XMFLOAT3(x + 1, y + 1, z), XMFLOAT2(1.0f, 0.0f), NEG_Z_NORMAL));
	vertices.push_back(Vertex(XMFLOAT3(x, y + 1, z), XMFLOAT2(0.0f, 0.0f), NEG_Z_NORMAL));

	indices.push_back(last_index + 0);
	indices.push_back(last_index + 2);
	indices.push_back(last_index + 1);

	indices.push_back(last_index + 0);
	indices.push_back(last_index + 3);
	indices.push_back(last_index + 2);
}

void Chunk::insertPositiveZ(float x, float y, float z, BlockType type, std::vector<Vertex>& vertices, std::vector<DWORD>& indices) {
	UINT last_index = vertices.size();
	vertices.push_back(Vertex(XMFLOAT3(x, y, z + 1), XMFLOAT2(1.0f, 1.0f), POS_Z_NORMAL));
	vertices.push_back(Vertex(XMFLOAT3(x + 1, y, z + 1), XMFLOAT2(1.0f, 0.0f), POS_Z_NORMAL));
	vertices.push_back(Vertex(XMFLOAT3(x + 1, y + 1, z + 1), XMFLOAT2(0.0f, 0.0f), POS_Z_NORMAL));
	vertices.push_back(Vertex(XMFLOAT3(x, y + 1, z + 1), XMFLOAT2(0.0f, 1.0f), POS_Z_NORMAL));

	indices.push_back(last_index + 0);
	indices.push_back(last_index + 1);
	indices.push_back(last_index + 2);

	indices.push_back(last_index + 0);
	indices.push_back(last_index + 2);
	indices.push_back(last_index + 3);
}

bool Chunk::isObscuredNegativeX(UINT x, UINT y, UINT z) {
	if (x > 0 && blocks_[x - 1][y][z] == BlockType::AIR) {
		return false;
	}
	else if (x == 0) {
		if (!left_ || (left_->getBlock(DIM - 1, y, z)) == BlockType::AIR) {
			return false;
		}
	}
	return true;
}

bool Chunk::isObscuredPositiveX(UINT x, UINT y, UINT z) {
	if (x + 1 < DIM && blocks_[x + 1][y][z] == BlockType::AIR) {
		return false;
	}
	else if (x + 1 == DIM) {
		if (!right_ || (right_->getBlock(0, y, z)) == BlockType::AIR) {
			return false;
		}
	}
	return true;
}

bool Chunk::isObscuredNegativeY(UINT x, UINT y, UINT z) {
	if (y > 0 && blocks_[x][y - 1][z] == BlockType::AIR) {
		return false;
	}
	else if (y == 0) {
		if (!down_ || (down_->getBlock(x, DIM - 1, z)) == BlockType::AIR) {
			return false;
		}
	}
	return true;
}

bool Chunk::isObscuredPositiveY(UINT x, UINT y, UINT z) {
	if (y + 1 < DIM && blocks_[x][y + 1][z] == BlockType::AIR) {
		return false;
	}
	else if (y == DIM - 1) {
		if (!up_ || (up_->getBlock(x, 0, z)) == BlockType::AIR) {
			return false;
		}
	}
	return true;
}

bool Chunk::isObscuredNegativeZ(UINT x, UINT y, UINT z) {
	if (z > 0 && blocks_[x][y][z - 1] == BlockType::AIR) {
		return false;
	}
	else if (z == 0) {
		if (!front_ || (front_->getBlock(x, y, DIM - 1)) == BlockType::AIR) {
			return false;
		}
	}
	return true;
}

bool Chunk::isObscuredPositiveZ(UINT x, UINT y, UINT z) {
	if (z + 1 < DIM && blocks_[x][y][z + 1] == BlockType::AIR) {
		return false;
	}
	else if (z + 1 == DIM) {
		if (!back_ || (back_->getBlock(x, y, 0)) == BlockType::AIR) {
			return false;
		}
	}
	return true;
}

void Chunk::calculateVertices(float x, float y, float z, BlockType type, std::vector<Vertex>& vertices, std::vector<DWORD>& indices) {
	if (!isObscuredNegativeX(x, y, z)) {
		insertNegativeX(x, y, z, type, vertices, indices);
	}
	if (!isObscuredPositiveX(x, y, z)) {
		insertPositiveX(x, y, z, type, vertices, indices);
	}
	if (!isObscuredNegativeY(x, y, z)) {
		insertNegativeY(x, y, z, type, vertices, indices);
	}
	if (!isObscuredPositiveY(x, y, z)) {
		insertPositiveY(x, y, z, type, vertices, indices);
	}
	if (!isObscuredNegativeZ(x, y, z)) {
		insertNegativeZ(x, y, z, type, vertices, indices);
	}
	if (!isObscuredPositiveZ(x, y, z)) {
		insertPositiveZ(x, y, z, type, vertices, indices);
	}
}

bool Chunk::isCovered(int x, int y, int z, int cov_x, int cov_y, int cov_z) {
	//TODO clean method or make it usefull
	if (blocks_[cov_x][cov_y][cov_z] != BlockType::AIR) {
		return true;
	}

	return false;
}
