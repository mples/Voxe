#include "TerrainMeshGeneratorSystem.h"
#include "../Components/MeshComponent.h"
#include "../Components/BoundingVolumeComponent.h"
#include "../Components/WorldCoordinateComponent.h"
#include <DirectXCollision.h>

TerrainMeshGenerationSystem::TerrainMeshGenerationSystem() : device_(nullptr), IEventListener(ENGINE.getEventHandler()) {
	std::function<void(const VoxelDataGeneratedEvent*)> voxel_data_callback = [&](const VoxelDataGeneratedEvent* e) {
		onVoxelDataGeneratedEvent(e);
	};
	std::function<void(const TerrainChunkChanged*)> chunk_changed_callback = [&](const TerrainChunkChanged* e) {
		onTerrainChunkChanged(e);
	};
	std::function<void(const DirectXDeviceCreated*)> device_created_callback = [&](const DirectXDeviceCreated* e) {
		onDirectXDeviceCreated(e);
	};
	registerEventCallback<VoxelDataGeneratedEvent>(voxel_data_callback);
	registerEventCallback<TerrainChunkChanged>(chunk_changed_callback);
	registerEventCallback<DirectXDeviceCreated>(device_created_callback);
}

TerrainMeshGenerationSystem::~TerrainMeshGenerationSystem() {
}

void TerrainMeshGenerationSystem::update(float dt) {
	if (device_ == nullptr) {
		return;
	}

	int updated_count = 0;

	while (!entitiesToUpdateMesh_.empty()) {
		if (updated_count == MAX_CHUNK_GENERATED_PER_UDPATE) {
			break;
		}

		EntityId& e_id = entitiesToUpdateMesh_.front();
		WorldCoordinateComponent * wc_comp = ENGINE.getComponentManager().getComponentByEntityId<WorldCoordinateComponent>(e_id);

		BlocksDataComponent * blocks_com = ENGINE.getComponentManager().getComponentByEntityId<BlocksDataComponent>(e_id);
		if (blocks_com != nullptr) {
			MeshComponent * mesh = ENGINE.getComponentManager().getComponentByEntityId<MeshComponent>(e_id);
			BoundingVolumeComponent * bv_com = ENGINE.getComponentManager().getComponentByEntityId<BoundingVolumeComponent>(e_id);
			if (blocks_com->isEmpty()) {
				if (mesh != nullptr) {
					ENGINE.getComponentManager().eraseComponent<MeshComponent>(e_id);
				}
				if (bv_com != nullptr) {
					ENGINE.getComponentManager().eraseComponent<BoundingVolumeComponent>(e_id);
				}
				entitiesToUpdateMesh_.pop_front();
				//updated_count++;
			}
			else {
				//WorldCoordinateComponent * wc_comp = ENGINE.getComponentManager().getComponentByEntityId<WorldCoordinateComponent>(e_id);

				if (bv_com == nullptr || mesh == nullptr) {
					entitiesToUpdateMesh_.pop_front();
					//entitiesToCreateMesh_.push_back(e_id);
					//updated_count++;
					continue;
				}

				TerrainNeightboursComponent * neight_comp = ENGINE.getComponentManager().getComponentByEntityId<TerrainNeightboursComponent>(e_id);
				if (neight_comp != nullptr) {
					NeightbourData neight_data;
					neight_data.left_ = ENGINE.getComponentManager().getComponentByEntityId<BlocksDataComponent>(neight_comp->left_);
					neight_data.right_ = ENGINE.getComponentManager().getComponentByEntityId<BlocksDataComponent>(neight_comp->right_);
					neight_data.top_ = ENGINE.getComponentManager().getComponentByEntityId<BlocksDataComponent>(neight_comp->top_);
					neight_data.bottom_ = ENGINE.getComponentManager().getComponentByEntityId<BlocksDataComponent>(neight_comp->bottom_);
					neight_data.front_ = ENGINE.getComponentManager().getComponentByEntityId<BlocksDataComponent>(neight_comp->front_);
					neight_data.back_ = ENGINE.getComponentManager().getComponentByEntityId<BlocksDataComponent>(neight_comp->back_);

					std::vector<Vertex> vertices;
					std::vector<DWORD> indices;

					calculateMeshData(vertices, indices, blocks_com, neight_data);
					mesh->clearMesh();
					if (vertices.size() != 0) {
						mesh->setMesh(device_, vertices, indices);
						BoundingBox bounding_box;
						bounding_box.CreateFromPoints(bounding_box, vertices.size(), &(vertices.data()->pos_), sizeof(Vertex));
						WorldCoordinateComponent * wc_comp = ENGINE.getComponentManager().getComponentByEntityId<WorldCoordinateComponent>(e_id);
						if (wc_comp) {
							bounding_box.Transform(bounding_box, wc_comp->getWorldMatrix());
						}
						bv_com->setBoundingVolume(bounding_box);
					}
				}
				else {
					int a = 0;
					a++;
				}
				entitiesToUpdateMesh_.pop_front();
				updated_count++;
			}
		}
		else {
			entitiesToUpdateMesh_.pop_front();

			//break;
		}
	}


	updated_count = 0;

	while (!entitiesToCreateMesh_.empty()) {
		if (updated_count == MAX_CHUNK_GENERATED_PER_UDPATE) {
			break;
		}

		EntityId & e_id = entitiesToCreateMesh_.front();
		WorldCoordinateComponent * wc_comp = ENGINE.getComponentManager().getComponentByEntityId<WorldCoordinateComponent>(e_id);

		BlocksDataComponent * blocks_com = ENGINE.getComponentManager().getComponentByEntityId<BlocksDataComponent>(e_id);
		TerrainNeightboursComponent * neight_comp = ENGINE.getComponentManager().getComponentByEntityId<TerrainNeightboursComponent>(e_id);
		if (blocks_com != nullptr && neight_comp != nullptr) {

			NeightbourData neight_data;
			neight_data.left_ = ENGINE.getComponentManager().getComponentByEntityId<BlocksDataComponent>(neight_comp->left_);
			neight_data.right_ = ENGINE.getComponentManager().getComponentByEntityId<BlocksDataComponent>(neight_comp->right_);
			neight_data.top_ = ENGINE.getComponentManager().getComponentByEntityId<BlocksDataComponent>(neight_comp->top_);
			neight_data.bottom_ = ENGINE.getComponentManager().getComponentByEntityId<BlocksDataComponent>(neight_comp->bottom_);
			neight_data.front_ = ENGINE.getComponentManager().getComponentByEntityId<BlocksDataComponent>(neight_comp->front_);
			neight_data.back_ = ENGINE.getComponentManager().getComponentByEntityId<BlocksDataComponent>(neight_comp->back_);

			std::vector<Vertex> vertices;
			std::vector<DWORD> indices;

			calculateMeshData(vertices, indices, blocks_com, neight_data);
			if (vertices.size() != 0) {
				if (neight_data.leftChanged_) {
					ENGINE.sendEvent<TerrainChunkChanged>(neight_data.left_->getOwner());
				}
				if (neight_data.rightChanged_) {
					ENGINE.sendEvent<TerrainChunkChanged>(neight_data.right_->getOwner());
				}
				if (neight_data.topChanged_) {
					ENGINE.sendEvent<TerrainChunkChanged>(neight_data.top_->getOwner());
				}
				if (neight_data.bottomChanged_) {
					ENGINE.sendEvent<TerrainChunkChanged>(neight_data.bottom_->getOwner());
				}
				if (neight_data.frontChanged_) {
					ENGINE.sendEvent<TerrainChunkChanged>(neight_data.front_->getOwner());
				}
				if (neight_data.backChanged_) {
					ENGINE.sendEvent<TerrainChunkChanged>(neight_data.back_->getOwner());
				}
				ENGINE.getComponentManager().addComponent<MeshComponent>(e_id, device_, vertices, indices);
				BoundingBox bounding_box;
				bounding_box.CreateFromPoints(bounding_box, vertices.size(), &(vertices.data()->pos_), sizeof(Vertex));
				WorldCoordinateComponent * wc_comp = ENGINE.getComponentManager().getComponentByEntityId<WorldCoordinateComponent>(e_id);
				if (wc_comp) {
					bounding_box.Transform(bounding_box, wc_comp->getWorldMatrix());
				}
				ENGINE.getComponentManager().addComponent<BoundingVolumeComponent>(e_id, bounding_box);
			}
			
			updated_count++;
			entitiesToCreateMesh_.pop_front();
		}
		else {
			entitiesToCreateMesh_.pop_front();
		}
	}
	//entitiesToCreateMesh_.clear();

	//entitiesToUpdateMesh_.clear();
}

void TerrainMeshGenerationSystem::onVoxelDataGeneratedEvent(const VoxelDataGeneratedEvent * e) {
	auto found = std::find(entitiesToCreateMesh_.begin(), entitiesToCreateMesh_.end(), e->id_);
	if (found == entitiesToCreateMesh_.end()) {
		entitiesToCreateMesh_.push_back(e->id_);
	}

}

void TerrainMeshGenerationSystem::onTerrainChunkChanged(const TerrainChunkChanged * e) {
	auto found = std::find(entitiesToUpdateMesh_.begin(), entitiesToUpdateMesh_.end(), e->id_);
	if (found == entitiesToUpdateMesh_.end()) {
		entitiesToUpdateMesh_.push_back(e->id_);
	}
}

void TerrainMeshGenerationSystem::onDirectXDeviceCreated(const DirectXDeviceCreated * e) {
	if (device_ == nullptr) {
		device_ = e->device_;
	}
	else {
		assert(0 && "Trying to set already set device pointer");
	}

}

void TerrainMeshGenerationSystem::calculateMeshData(std::vector<Vertex>& vertices, std::vector<DWORD>& indices, BlocksDataComponent * blocks_com, NeightbourData& neight_data) {
	for (UINT x = 0; x < TERRAIN_CHUNK_DIM; ++x) {
		for (UINT y = 0; y < TERRAIN_CHUNK_DIM; ++y) {
			for (UINT z = 0; z < TERRAIN_CHUNK_DIM; ++z) {
				BlockType type = blocks_com->getBlock(x,y,z);
				if (type != BlockType::AIR) {
					calculateVertices(x, y, z, type, vertices, indices, blocks_com, neight_data);
				}
			}
		}
	}
}

void TerrainMeshGenerationSystem::calculateVertices(UINT x, UINT y, UINT z, BlockType type, std::vector<Vertex>& vertices, std::vector<DWORD>& indices, BlocksDataComponent * blocks_com, NeightbourData& neight_data) {
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

bool TerrainMeshGenerationSystem::isObscuredNegativeX(UINT x, UINT y, UINT z, BlocksDataComponent * blocks_com, NeightbourData& neight_data) {
	if (x > 0 && blocks_com->getBlock(x - 1, y, z) == BlockType::AIR) {
		return false;
	}
	else if (x == 0) {
		if (neight_data.left_ == nullptr || neight_data.left_->getBlock(TERRAIN_CHUNK_DIM - 1, y, z) == BlockType::AIR ) {
			return false;
		}
		else {
			if (!neight_data.leftChanged_) {
				neight_data.leftChanged_ = true;
			}
		}
	}
	return true;
}

bool TerrainMeshGenerationSystem::isObscuredPositiveX(UINT x, UINT y, UINT z, BlocksDataComponent * blocks_com, NeightbourData& neight_data) {
	if (x + 1 < TERRAIN_CHUNK_DIM && blocks_com->getBlock(x + 1, y, z) == BlockType::AIR) {
		return false;
	}
	else if (x + 1 == TERRAIN_CHUNK_DIM) {
		if (neight_data.right_ == nullptr || neight_data.right_->getBlock(0, y, z) == BlockType::AIR) {
			return false;
		}
		else {
			if (!neight_data.rightChanged_) {
				neight_data.rightChanged_ = true;
			}
		}
	}
	return true;
}

bool TerrainMeshGenerationSystem::isObscuredNegativeY(UINT x, UINT y, UINT z, BlocksDataComponent * blocks_com, NeightbourData& neight_data) {
	if (y > 0 && blocks_com->getBlock(x, y - 1, z) == BlockType::AIR) {
		return false;
	}
	else if (y == 0) {
		if (neight_data.bottom_ == nullptr || neight_data.bottom_->getBlock(x, TERRAIN_CHUNK_DIM - 1, z) == BlockType::AIR) {
			return false;
		}
		else {
			if (!neight_data.bottomChanged_) {
				neight_data.bottomChanged_ = true;
			}
		}
	}
	return true;
}

bool TerrainMeshGenerationSystem::isObscuredPositiveY(UINT x, UINT y, UINT z, BlocksDataComponent * blocks_com, NeightbourData& neight_data) {
	if (y + 1 < TERRAIN_CHUNK_DIM && blocks_com->getBlock(x, y + 1, z) == BlockType::AIR) {
		return false;
	}
	else if (y == TERRAIN_CHUNK_DIM - 1) {
		if (neight_data.top_ == nullptr || neight_data.top_->getBlock(x, 0, z) == BlockType::AIR) {
			return false;
		}
		else {
			if (!neight_data.topChanged_) {
				neight_data.topChanged_ = true;
			}
		}
	}
	return true;
}

bool TerrainMeshGenerationSystem::isObscuredNegativeZ(UINT x, UINT y, UINT z, BlocksDataComponent * blocks_com, NeightbourData& neight_data) {
	if (z > 0 && blocks_com->getBlock(x, y, z - 1) == BlockType::AIR) {
		return false;
	}
	else if (z == 0) {
		if (neight_data.back_ == nullptr || neight_data.back_->getBlock(x, y, TERRAIN_CHUNK_DIM - 1) == BlockType::AIR) {
			return false;
		}
		else {
			if (!neight_data.backChanged_) {
				neight_data.backChanged_ = true;
			}
		}
	}
	return true;
}

bool TerrainMeshGenerationSystem::isObscuredPositiveZ(UINT x, UINT y, UINT z, BlocksDataComponent * blocks_com, NeightbourData& neight_data) {
	if (z + 1 < TERRAIN_CHUNK_DIM && blocks_com->getBlock(x, y, z + 1) == BlockType::AIR) {
		return false;
	}
	else if (z + 1 == TERRAIN_CHUNK_DIM) {
		if (neight_data.front_ == nullptr || neight_data.front_->getBlock(x, y, 0) == BlockType::AIR) {
			return false;
		}
		else {
			if (!neight_data.frontChanged_) {
				neight_data.frontChanged_ = true;
			}
		}
	}
	return true;
}
