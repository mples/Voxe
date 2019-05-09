#include "Chunk.h"
#include "../BoundingVolume.h"

Chunk::Chunk() : coord_(0, 0, 0), changed_(false), isEmpty_(false), initialized_(false) {
	ZeroMemory(blocks_, pow(DIM, 3) * sizeof(BlockType));
	worldMatrix_ = XMMatrixTranslation(static_cast<float>(coord_.x_ * static_cast<int>(Chunk::DIM)),
		static_cast<float>(coord_.y_ * static_cast<int>(Chunk::DIM)),
		static_cast<float>(coord_.z_ * static_cast<int>(Chunk::DIM)));
	neighbours_.back_ = nullptr;
	neighbours_.front_ = nullptr;
	neighbours_.left_ = nullptr;
	neighbours_.right_ = nullptr;
	neighbours_.up_ = nullptr;
	neighbours_.down_ = nullptr;
}

Chunk::Chunk(int x, int y, int z) : changed_(false), coord_(x, y, z), initialized_(false), isEmpty_(false) {
	ZeroMemory(blocks_, pow(DIM, 3) * sizeof(BlockType));
	worldMatrix_ = XMMatrixTranslation(static_cast<float>(coord_.x_ * static_cast<int>(Chunk::DIM)),
										static_cast<float>(coord_.y_ * static_cast<int>(Chunk::DIM)),
										static_cast<float>(coord_.z_ * static_cast<int>(Chunk::DIM)));
	neighbours_.back_ = nullptr;
	neighbours_.front_ = nullptr;
	neighbours_.left_ = nullptr;
	neighbours_.right_ = nullptr;
	neighbours_.up_ = nullptr;
	neighbours_.down_ = nullptr;
}

Chunk::Chunk(int x, int y, int z, BlockType blocks[Chunk::DIM][Chunk::DIM][Chunk::DIM]) : changed_(false), coord_(x, y, z), initialized_(false), isEmpty_(false) {
	CopyMemory(blocks_, blocks, sizeof(BlockType) * DIM * DIM * DIM);
	worldMatrix_ = XMMatrixTranslation(static_cast<float>(coord_.x_ * static_cast<int>(Chunk::DIM)),
										static_cast<float>(coord_.y_ * static_cast<int>(Chunk::DIM)),
										static_cast<float>(coord_.z_ * static_cast<int>(Chunk::DIM)));
	neighbours_.back_ = nullptr;
	neighbours_.front_ = nullptr;
	neighbours_.left_ = nullptr;
	neighbours_.right_ = nullptr;
	neighbours_.up_ = nullptr;
	neighbours_.down_ = nullptr;
}


Chunk::Chunk(ChunkCoord coord, BlockType blocks[Chunk::DIM][Chunk::DIM][Chunk::DIM]) : coord_(coord), initialized_(false), isEmpty_(false) {
	CopyMemory(blocks_, blocks, sizeof(BlockType) * DIM * DIM * DIM);
	worldMatrix_ = XMMatrixTranslation(static_cast<float>(coord_.x_ * static_cast<int>(Chunk::DIM)),
		static_cast<float>(coord_.y_ * static_cast<int>(Chunk::DIM)),
		static_cast<float>(coord_.z_ * static_cast<int>(Chunk::DIM)));
	neighbours_.back_ = nullptr;
	neighbours_.front_ = nullptr;
	neighbours_.left_ = nullptr;
	neighbours_.right_ = nullptr;
	neighbours_.up_ = nullptr;
	neighbours_.down_ = nullptr;
}

Chunk::~Chunk() {
}

void Chunk::initialize() {
	coord_ = ChunkCoord(0, 0, 0);
	changed_ = false;
	initialized_ = false;
	isEmpty_ = false;

	ZeroMemory(blocks_, pow(DIM, 3) * sizeof(BlockType));
	worldMatrix_ = XMMatrixTranslation(static_cast<float>(coord_.x_ * static_cast<int>(Chunk::DIM)),
		static_cast<float>(coord_.y_ * static_cast<int>(Chunk::DIM)),
		static_cast<float>(coord_.z_ * static_cast<int>(Chunk::DIM)));
	neighbours_.back_ = nullptr;
	neighbours_.front_ = nullptr;
	neighbours_.left_ = nullptr;
	neighbours_.right_ = nullptr;
	neighbours_.up_ = nullptr;
	neighbours_.down_ = nullptr;
}

void Chunk::initialize(int x, int y, int z, BlockType blocks[Chunk::DIM][Chunk::DIM][Chunk::DIM]) {
	coord_ = ChunkCoord(x,y,z);
	CopyMemory(blocks_, blocks, sizeof(BlockType) * DIM * DIM * DIM);
	worldMatrix_ = XMMatrixTranslation(static_cast<float>(coord_.x_ * static_cast<int>(Chunk::DIM)),
		static_cast<float>(coord_.y_ * static_cast<int>(Chunk::DIM)),
		static_cast<float>(coord_.z_ * static_cast<int>(Chunk::DIM)));
	neighbours_.back_ = nullptr;
	neighbours_.front_ = nullptr;
	neighbours_.left_ = nullptr;
	neighbours_.right_ = nullptr;
	neighbours_.up_ = nullptr;
	neighbours_.down_ = nullptr;
}

void Chunk::initialize(ChunkCoord coord, BlockType blocks[Chunk::DIM][Chunk::DIM][Chunk::DIM]) {
	coord_ = coord;
	CopyMemory(blocks_, blocks, sizeof(BlockType) * DIM * DIM * DIM);
	worldMatrix_ = XMMatrixTranslation(static_cast<float>(coord_.x_ * static_cast<int>(Chunk::DIM)),
		static_cast<float>(coord_.y_ * static_cast<int>(Chunk::DIM)),
		static_cast<float>(coord_.z_ * static_cast<int>(Chunk::DIM)));
	neighbours_.back_ = nullptr;
	neighbours_.front_ = nullptr;
	neighbours_.left_ = nullptr;
	neighbours_.right_ = nullptr;
	neighbours_.up_ = nullptr;
	neighbours_.down_ = nullptr;
}

bool Chunk::initializeMesh(ID3D11Device * device, ID3D11DeviceContext * device_context, ConstantBuffer<CB_VS_object_buffer>& const_buffer, Texture * texture) {
	if (isEmpty_) {
		return false;
	}
	std::vector<Vertex> vertices;
	std::vector<DWORD> indices;

	calculateMeshData(vertices, indices);
	if (vertices.size() > 0) {
		try {
			model_.initialize(device, device_context, const_buffer, vertices, indices, texture);
		}
		catch (COMException e) {
			ErrorLogger::log(e);
		}
		initialized_ = true;
	}
	else {
		isEmpty_ = true;
		initialized_ = true;
	}
	model_.getBoundingBox().Transform(worldBoundingBox_, worldMatrix_);
	boundingVolume_ = BoundingVolume(getPos(), worldBoundingBox_);
	return true;
}

void Chunk::update(ID3D11Device * device, ID3D11DeviceContext * device_context, ConstantBuffer<CB_VS_object_buffer>& const_buffer, Texture * texture) {
	if (initialized_ == false) {
		return;
	}

	model_.reset();
	initialized_ = false;
	isEmpty_ = false;
	initializeMesh(device, device_context, const_buffer, texture);
	initialized_ = true;

	changed_ = false;
}

void Chunk::draw(XMMATRIX view_proj_matrix) {
	//if (changed_) {
		//update();
	//}
	if (!initialized_ || isEmpty_) {
		return;
	}

	model_.draw(worldMatrix_, view_proj_matrix);

}

void Chunk::setBlock(int x, int y, int z, BlockType type) {
	//std::cout << "Chunk x: " << x << " y: " << y << " z: " << z << std::endl;
	if (x < 0 || x >= DIM || y < 0 || y >= DIM || z < 0 || z >= DIM)
		return;
	blocks_[x][y][z] = type;
	changed_ = true;

	if (x == 0 && neighbours_.left_ != nullptr) {
		neighbours_.left_->changed_ = true;
	}else if (x == DIM - 1 && neighbours_.right_ != nullptr) {
		neighbours_.right_->changed_ = true;
	}
	if (y == DIM - 1 && neighbours_.up_ != nullptr) {
		neighbours_.up_->changed_ = true;
	}
	else if (y == 0 && neighbours_.down_ != nullptr) {
		neighbours_.down_->changed_ = true;
	}
	if (z == 0 && neighbours_.back_ != nullptr) {
		neighbours_.back_->changed_ = true;
	}
	else if (z == DIM - 1 && neighbours_.front_ != nullptr) {
		neighbours_.front_->changed_ = true;
	}
}

BlockType Chunk::getBlock(UINT x, UINT y, UINT z) {
	return blocks_[x][y][z];
}

BoundingBox & Chunk::getBoundingVolume() {
	return worldBoundingBox_;
}

XMMATRIX & Chunk::getWorldMatrix() {
	return worldMatrix_;
}

ChunkCoord & Chunk::getCoord() {
	return coord_;
}

XMINT3 Chunk::getPos() {
	return XMINT3(coord_.x_, coord_.y_, coord_.z_);
}

BoundingVolume & Chunk::getBV() {
	return boundingVolume_;
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
		if (!neighbours_.left_ || (neighbours_.left_->getBlock(Chunk::DIM - 1, y, z)) == BlockType::AIR) {
			return false;
		}
	}
	return true;
}

bool Chunk::isObscuredPositiveX(UINT x, UINT y, UINT z) {
	if (x + 1 < Chunk::DIM && blocks_[x + 1][y][z] == BlockType::AIR) {
		return false;
	}
	else if (x + 1 == Chunk::DIM) {
		if (!neighbours_.right_ || (neighbours_.right_->getBlock(0, y, z)) == BlockType::AIR) {
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
		if (!neighbours_.down_ || (neighbours_.down_->getBlock(x, Chunk::DIM - 1, z)) == BlockType::AIR) {
			return false;
		}
	}
	return true;
}

bool Chunk::isObscuredPositiveY(UINT x, UINT y, UINT z) {
	if (y + 1 < Chunk::DIM && blocks_[x][y + 1][z] == BlockType::AIR) {
		return false;
	}
	else if (y == Chunk::DIM - 1) {
		if (!neighbours_.up_ || (neighbours_.up_->getBlock(x, 0, z)) == BlockType::AIR) {
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
		if (!neighbours_.back_ || (neighbours_.back_->getBlock(x, y, Chunk::DIM - 1)) == BlockType::AIR) {
			return false;
		}
	}
	return true;
}

bool Chunk::isObscuredPositiveZ(UINT x, UINT y, UINT z) {
	if (z + 1 < Chunk::DIM && blocks_[x][y][z + 1] == BlockType::AIR) {
		return false;
	}
	else if (z + 1 == Chunk::DIM) {
		if (!neighbours_.front_ || (neighbours_.front_->getBlock(x, y, 0)) == BlockType::AIR) {
			return false;
		}
	}
	return true;
}

void Chunk::calculateVertices(UINT x, UINT y, UINT z, BlockType type, std::vector<Vertex>& vertices, std::vector<DWORD>& indices) {
	if (!isObscuredNegativeX(x, y, z)) {
		insertNegativeX(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), type, vertices, indices);
	}
	if (!isObscuredPositiveX(x, y, z)) {
		insertPositiveX(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), type, vertices, indices);
	}
	if (!isObscuredNegativeY(x, y, z)) {
		insertNegativeY(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), type, vertices, indices);
	}
	if (!isObscuredPositiveY(x, y, z)) {
		insertPositiveY(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), type, vertices, indices);
	}
	if (!isObscuredNegativeZ(x, y, z)) {
		insertNegativeZ(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), type, vertices, indices);
	}
	if (!isObscuredPositiveZ(x, y, z)) {
		insertPositiveZ(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), type, vertices, indices);
	}
}

void Chunk::calculateMeshData(std::vector<Vertex>& vertices, std::vector<DWORD>& indices) {
	for (UINT x = 0; x < Chunk::DIM; ++x) {
		for (UINT y = 0; y < Chunk::DIM; ++y) {
			for (UINT z = 0; z < Chunk::DIM; ++z) {
				BlockType type = blocks_[x][y][z];
				if (type != BlockType::AIR) {
					calculateVertices(x, y, z, type, vertices, indices);
				}
			}
		}
	}
}
