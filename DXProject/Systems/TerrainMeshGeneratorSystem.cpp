#include "TerrainMeshGeneratorSystem.h"
#include "../Components/MeshComponent.h"

TerrainMeshGenerationSystem::TerrainMeshGenerationSystem() : IEventListener(ENGINE.getEventHandler()) {
	std::function<void(const VoxelDataGeneratedEvent*)> f = [&](const VoxelDataGeneratedEvent* e) {
		onVoxelDataGeneratedEvent(e);
	};
	registerEventCallback<VoxelDataGeneratedEvent>(f);
}

TerrainMeshGenerationSystem::~TerrainMeshGenerationSystem() {
}

void TerrainMeshGenerationSystem::update(float dt) {
	auto it = entitiesToUpdate_.begin();
	while (it != entitiesToUpdate_.end()) {
		BlocksDataComponent * blocks_com = ENGINE.getComponentManager().getComponent<BlocksDataComponent>(*it);
		if (blocks_com != nullptr) {
			TerrainNeightboursComponent * neight_comp = ENGINE.getComponentManager().getComponent<TerrainNeightboursComponent>(*it);
			if (neight_comp != nullptr) {
				NeightbourData neight_data;
				neight_data.left_ = ENGINE.getComponentManager().getComponent<BlocksDataComponent>(neight_comp->left_);
				neight_data.right_ = ENGINE.getComponentManager().getComponent<BlocksDataComponent>(neight_comp->right_);
				neight_data.top_ = ENGINE.getComponentManager().getComponent<BlocksDataComponent>(neight_comp->top_);
				neight_data.bottom_ = ENGINE.getComponentManager().getComponent<BlocksDataComponent>(neight_comp->bottom_);
				neight_data.front_ = ENGINE.getComponentManager().getComponent<BlocksDataComponent>(neight_comp->front_);
				neight_data.back_ = ENGINE.getComponentManager().getComponent<BlocksDataComponent>(neight_comp->back_);

				std::vector<Vertex> vertices;
				std::vector<DWORD> indices;

				calculateMeshData(vertices, indices, blocks_com, neight_data);

				ENGINE.getComponentManager().addComponent<MeshComponent>(*it, device_, vertices, indices);
			}
		}
		it++;
	}
	entitiesToUpdate_.clear();
}

void TerrainMeshGenerationSystem::onVoxelDataGeneratedEvent(const VoxelDataGeneratedEvent * e) {
	entitiesToUpdate_.push_back(e->id_);
}

void TerrainMeshGenerationSystem::calculateMeshData(std::vector<Vertex>& vertices, std::vector<DWORD>& indices, BlocksDataComponent * blocks_com, NeightbourData neight_data) {
	for (UINT x = 0; x < DIMENSION; ++x) {
		for (UINT y = 0; y < DIMENSION; ++y) {
			for (UINT z = 0; z < DIMENSION; ++z) {
				BlockType type = blocks_com->getBlock(x,y,z);
				if (type != BlockType::AIR) {
					calculateVertices(x, y, z, type, vertices, indices, blocks_com, neight_data);
				}
			}
		}
	}
}

void TerrainMeshGenerationSystem::calculateVertices(UINT x, UINT y, UINT z, BlockType type, std::vector<Vertex>& vertices, std::vector<DWORD>& indices, BlocksDataComponent * blocks_com, NeightbourData neight_data) {
	if (!isObscuredNegativeX(x, y, z, blocks_com, neight_data)) {
		insertNegativeX(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), type, vertices, indices);
	}
	if (!isObscuredPositiveX(x, y, z, blocks_com, neight_data)) {
		insertPositiveX(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), type, vertices, indices);
	}
	if (!isObscuredNegativeY(x, y, z, blocks_com, neight_data)) {
		insertNegativeY(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), type, vertices, indices);
	}
	if (!isObscuredPositiveY(x, y, z, blocks_com, neight_data)) {
		insertPositiveY(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), type, vertices, indices);
	}
	if (!isObscuredNegativeZ(x, y, z, blocks_com, neight_data)) {
		insertNegativeZ(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), type, vertices, indices);
	}
	if (!isObscuredPositiveZ(x, y, z, blocks_com, neight_data)) {
		insertPositiveZ(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z), type, vertices, indices);
	}
}

void TerrainMeshGenerationSystem::insertNegativeX(float x, float y, float z, BlockType type, std::vector<Vertex>& vertices, std::vector<DWORD>& indices) {
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

void TerrainMeshGenerationSystem::insertPositiveX(float x, float y, float z, BlockType type, std::vector<Vertex>& vertices, std::vector<DWORD>& indices) {
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

void TerrainMeshGenerationSystem::insertNegativeY(float x, float y, float z, BlockType type, std::vector<Vertex>& vertices, std::vector<DWORD>& indices) {
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

void TerrainMeshGenerationSystem::insertPositiveY(float x, float y, float z, BlockType type, std::vector<Vertex>& vertices, std::vector<DWORD>& indices) {
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

void TerrainMeshGenerationSystem::insertNegativeZ(float x, float y, float z, BlockType type, std::vector<Vertex>& vertices, std::vector<DWORD>& indices) {
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

void TerrainMeshGenerationSystem::insertPositiveZ(float x, float y, float z, BlockType type, std::vector<Vertex>& vertices, std::vector<DWORD>& indices) {
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

bool TerrainMeshGenerationSystem::isObscuredNegativeX(UINT x, UINT y, UINT z, BlocksDataComponent * blocks_com, NeightbourData neight_data) {
	if (x > 0 && blocks_com->getBlock(x - 1, y, z) == BlockType::AIR) {
		return false;
	}
	else if (x == 0) {
		if (neight_data.left_ != nullptr || neight_data.left_->getBlock(DIMENSION - 1, y, z) == BlockType::AIR ) {
			return false;
		}
	}
	return true;
}

bool TerrainMeshGenerationSystem::isObscuredPositiveX(UINT x, UINT y, UINT z, BlocksDataComponent * blocks_com, NeightbourData neight_data) {
	if (x + 1 < DIMENSION && blocks_com->getBlock(x + 1, y, z) == BlockType::AIR) {
		return false;
	}
	else if (x + 1 == DIMENSION) {
		if (neight_data.right_ != nullptr || neight_data.right_->getBlock(0, y, z) == BlockType::AIR) {
			return false;
		}
	}
	return true;
}

bool TerrainMeshGenerationSystem::isObscuredNegativeY(UINT x, UINT y, UINT z, BlocksDataComponent * blocks_com, NeightbourData neight_data) {
	if (y > 0 && blocks_com->getBlock(x, y - 1, z) == BlockType::AIR) {
		return false;
	}
	else if (y == 0) {
		if (neight_data.bottom_ != nullptr || neight_data.bottom_->getBlock(x, DIMENSION - 1, z) == BlockType::AIR) {
			return false;
		}
	}
	return true;
}

bool TerrainMeshGenerationSystem::isObscuredPositiveY(UINT x, UINT y, UINT z, BlocksDataComponent * blocks_com, NeightbourData neight_data) {
	if (y + 1 < DIMENSION && blocks_com->getBlock(x, y + 1, z) == BlockType::AIR) {
		return false;
	}
	else if (y == DIMENSION - 1) {
		if (neight_data.top_ != nullptr || neight_data.top_->getBlock(x, 0, z) == BlockType::AIR) {
			return false;
		}
	}
	return true;
}

bool TerrainMeshGenerationSystem::isObscuredNegativeZ(UINT x, UINT y, UINT z, BlocksDataComponent * blocks_com, NeightbourData neight_data) {
	if (z > 0 && blocks_com->getBlock(x, y, z - 1) == BlockType::AIR) {
		return false;
	}
	else if (z == 0) {
		if (neight_data.back_ != nullptr || neight_data.back_->getBlock(x, y, DIMENSION - 1) == BlockType::AIR) {
			return false;
		}
	}
	return true;
}

bool TerrainMeshGenerationSystem::isObscuredPositiveZ(UINT x, UINT y, UINT z, BlocksDataComponent * blocks_com, NeightbourData neight_data) {
	if (z + 1 < DIMENSION && blocks_com->getBlock(x, y, z + 1) == BlockType::AIR) {
		return false;
	}
	else if (z + 1 == DIMENSION) {
		if (neight_data.front_ != nullptr || neight_data.front_->getBlock(x, y, 0) == BlockType::AIR) {
			return false;
		}
	}
	return true;
}
