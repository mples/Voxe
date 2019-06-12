#include "InfiniteWorldSystem.h"
#include "../Entities/GameCamera.h"
#include "../Voxel/BlockType.h"
#include "../Events/TerrainChunkRequest.h"
#include "../Events/TerrainChunkDestroyedEvent.h"
#include "../Engine.h"
#include "../Components/WorldCoordinateComponent.h"

InfiniteWorldSystem::InfiniteWorldSystem() : IEventListener(ENGINE.getEventHandler()), activeCamera_(nullptr), initialGeneration_(false) {
	registerEventCallback<CameraCreated>([&](const CameraCreated * e) {
		onCameraCreated(e);
	});

	registerEventCallback<CameraDestroyed>([&](const CameraDestroyed * e) {
		onCameraDestroyed(e);
	});
}

InfiniteWorldSystem::~InfiniteWorldSystem() {
}

void InfiniteWorldSystem::preUpdate(float dt) {
	if (activeCamera_ == nullptr) {
		return;
	}
	if (initialGeneration_ == false) {
		originPosition_ = activeCamera_->getCameraPosition();
		originCoord_ = XMINT3(static_cast<int>(originPosition_.x / TERRAIN_CHUNK_DIM), static_cast<int>(originPosition_.y / TERRAIN_CHUNK_DIM), static_cast<int>(originPosition_.z / TERRAIN_CHUNK_DIM));

		for (int x = originCoord_.x - (1.5 * size_); x < originCoord_.x + (1.5 * size_); x++) {
			for (int y = originCoord_.y - (1.5 * size_); y < originCoord_.y + (1.5 * size_); y++) {
				for (int z = originCoord_.z - (1.5 * size_); z < originCoord_.z + (1.5 * size_); z++) {
					createRequests_.push(XMINT3(x, y, z));
				}
			}
		}
		initialGeneration_ = true;
	}
}

void InfiniteWorldSystem::update(float dt) {
	if (activeCamera_ == nullptr) {
		return;
	}

	for (auto e_id : terrainChunksGenerated_) {
		WorldCoordinateComponent * coord = ENGINE.getComponentManager().getComponentByEntityId<WorldCoordinateComponent>(e_id);
		if (coord != nullptr) {
			XMINT3 hashed_coord = hashWorldCoord(coord->getCoord());
			activeWorldChunks_.at(hashed_coord.x, hashed_coord.y, hashed_coord.z).push_back(e_id);
		}

	}
	terrainChunksGenerated_.clear();

	XMVECTOR origin = XMLoadFloat3(&activeCamera_->getCameraPosition());
	XMVECTOR prev_center = XMLoadFloat3(&originPosition_);
	XMVECTOR moved_distance = XMVectorSubtract(origin, prev_center);
	XMFLOAT3 move_dist;

	XMStoreFloat3(&move_dist, moved_distance);

	if (move_dist.x > static_cast<float>((size_ / 2) * TERRAIN_CHUNK_DIM)) {
		OutputDebugStringA("Right entends.\n");
		
		for (int x = originCoord_.x - (1.5 * size_); x < originCoord_.x - (0.5 * size_); x++) {
			for (int y = originCoord_.y - (1.5 * size_); y < originCoord_.y + (1.5 * size_); y++) {
				for (int z = originCoord_.z - (1.5 * size_); z < originCoord_.z + (1.5 * size_); z++) {
					deleteRequests_.push(XMINT3(x, y, z));
				}
			}
		}

		originPosition_.x += size_ * TERRAIN_CHUNK_DIM;
		originCoord_ = XMINT3(static_cast<int>(originPosition_.x / TERRAIN_CHUNK_DIM), static_cast<int>(originPosition_.y / TERRAIN_CHUNK_DIM), static_cast<int>(originPosition_.z / TERRAIN_CHUNK_DIM));

		for (int x = originCoord_.x + (0.5 * size_); x < originCoord_.x + (1.5 * size_); x++) {
			for (int y = originCoord_.y - (1.5 * size_); y < originCoord_.y + (1.5 * size_); y++) {
				for (int z = originCoord_.z - (1.5 * size_); z < originCoord_.z + (1.5 * size_); z++) {
					createRequests_.push(XMINT3(x,y,z));
				}
			}
		}

	}
	else if (move_dist.x < -1.0f * static_cast<float>((size_ / 2) * TERRAIN_CHUNK_DIM)) {
		OutputDebugStringA("Left entends.\n");

		for (int x = originCoord_.x + (0.5 * size_); x < originCoord_.x + (1.5 * size_); x++) {
			for (int y = originCoord_.y - (1.5 * size_); y < originCoord_.y + (1.5 * size_); y++) {
				for (int z = originCoord_.z - (1.5 * size_); z < originCoord_.z + (1.5 * size_); z++) {
					deleteRequests_.push(XMINT3(x, y, z));
				}
			}
		}

		originPosition_.x -= size_ * TERRAIN_CHUNK_DIM;
		originCoord_ = XMINT3(static_cast<int>(originPosition_.x / TERRAIN_CHUNK_DIM), static_cast<int>(originPosition_.y / TERRAIN_CHUNK_DIM), static_cast<int>(originPosition_.z / TERRAIN_CHUNK_DIM));

		for (int x = originCoord_.x - (1.5 * size_); x < originCoord_.x - (0.5 * size_); x++) {
			for (int y = originCoord_.y - (1.5 * size_); y < originCoord_.y + (1.5 * size_); y++) {
				for (int z = originCoord_.z - (1.5 * size_); z < originCoord_.z + (1.5 * size_); z++) {
					createRequests_.push(XMINT3(x, y, z));
				}
			}
		}
	}
	else if (move_dist.y > static_cast<float>((size_ / 2) * TERRAIN_CHUNK_DIM)) {
		OutputDebugStringA("Up entends.\n");

		for (int x = originCoord_.x - (1.5 * size_); x < originCoord_.x + (1.5 * size_); x++) {
			for (int y = originCoord_.y - (1.5 * size_); y < originCoord_.y - (0.5 * size_); y++) {
				for (int z = originCoord_.z - (1.5 * size_); z < originCoord_.z + (1.5 * size_); z++) {
					deleteRequests_.push(XMINT3(x, y, z));
				}
			}
		}

		originPosition_.y += size_ * TERRAIN_CHUNK_DIM;
		originCoord_ = XMINT3(static_cast<int>(originPosition_.x / TERRAIN_CHUNK_DIM), static_cast<int>(originPosition_.y / TERRAIN_CHUNK_DIM), static_cast<int>(originPosition_.z / TERRAIN_CHUNK_DIM));

		for (int x = originCoord_.x - (1.5 * size_); x < originCoord_.x + (1.5 * size_); x++) {
			for (int y = originCoord_.y + (0.5 * size_); y < originCoord_.y + (1.5 * size_); y++) {
				for (int z = originCoord_.z - (1.5 * size_); z < originCoord_.z + (1.5 * size_); z++) {
					createRequests_.push(XMINT3(x, y, z));
				}
			}
		}

	}
	else if (move_dist.y < -1.0f * static_cast<float>((size_ / 2) * TERRAIN_CHUNK_DIM)) {
		OutputDebugStringA("Down entends.\n");

		for (int x = originCoord_.x - (1.5 * size_); x < originCoord_.x + (1.5 * size_); x++) {
			for (int y = originCoord_.y + (0.5 * size_); y < originCoord_.y + (1.5 * size_); y++) {
				for (int z = originCoord_.z - (1.5 * size_); z < originCoord_.z + (1.5 * size_); z++) {
					deleteRequests_.push(XMINT3(x, y, z));
				}
			}
		}

		originPosition_.y -= size_ * TERRAIN_CHUNK_DIM;
		originCoord_ = XMINT3(static_cast<int>(originPosition_.x / TERRAIN_CHUNK_DIM), static_cast<int>(originPosition_.y / TERRAIN_CHUNK_DIM), static_cast<int>(originPosition_.z / TERRAIN_CHUNK_DIM));

		for (int x = originCoord_.x - (1.5 * size_); x < originCoord_.x + (1.5 * size_); x++) {
			for (int y = originCoord_.y - (1.5 * size_); y < originCoord_.y - (0.5 * size_); y++) {
				for (int z = originCoord_.z - (1.5 * size_); z < originCoord_.z + (1.5 * size_); z++) {
					createRequests_.push(XMINT3(x, y, z));
				}
			}
		}

	}
	else if (move_dist.z > static_cast<float>((size_ / 2) * TERRAIN_CHUNK_DIM)) {
		OutputDebugStringA("Front entends.\n");

		for (int x = originCoord_.x - (1.5 * size_); x < originCoord_.x + (1.5 * size_); x++) {
			for (int y = originCoord_.y - (1.5 * size_); y < originCoord_.y + (1.5 * size_); y++) {
				for (int z = originCoord_.z - (1.5 * size_); z < originCoord_.z - (0.5 * size_); z++) {
					deleteRequests_.push(XMINT3(x, y, z));
				}
			}
		}

		originPosition_.z += size_ * TERRAIN_CHUNK_DIM;
		originCoord_ = XMINT3(static_cast<int>(originPosition_.x / TERRAIN_CHUNK_DIM), static_cast<int>(originPosition_.y / TERRAIN_CHUNK_DIM), static_cast<int>(originPosition_.z / TERRAIN_CHUNK_DIM));

		for (int x = originCoord_.x - (1.5 * size_); x < originCoord_.x + (1.5 * size_); x++) {
			for (int y = originCoord_.y - (1.5 * size_); y < originCoord_.y + (1.5 * size_); y++) {
				for (int z = originCoord_.z + (0.5 * size_); z < originCoord_.z + (1.5 * size_); z++) {
					createRequests_.push(XMINT3(x, y, z));
				}
			}
		}

	}
	else if (move_dist.z < -1.0f * static_cast<float>((size_ / 2) * TERRAIN_CHUNK_DIM)) {
		OutputDebugStringA("Back entends.\n");

		for (int x = originCoord_.x - (1.5 * size_); x < originCoord_.x + (1.5 * size_); x++) {
			for (int y = originCoord_.y - (1.5 * size_); y < originCoord_.y + (1.5 * size_); y++) {
				for (int z = originCoord_.z + (0.5 * size_); z < originCoord_.z + (1.5 * size_); z++) {
					deleteRequests_.push(XMINT3(x, y, z));
				}
			}
		}
		originPosition_.z -= size_ * TERRAIN_CHUNK_DIM;
		originCoord_ = XMINT3(static_cast<int>(originPosition_.x / TERRAIN_CHUNK_DIM), static_cast<int>(originPosition_.y / TERRAIN_CHUNK_DIM), static_cast<int>(originPosition_.z / TERRAIN_CHUNK_DIM));

		for (int x = originCoord_.x - (1.5 * size_); x < originCoord_.x + (1.5 * size_); x++) {
			for (int y = originCoord_.y - (1.5 * size_); y < originCoord_.y + (1.5 * size_); y++) {
				for (int z = originCoord_.z - (1.5 * size_); z < originCoord_.z - (0.5 * size_); z++) {
					createRequests_.push(XMINT3(x, y, z));
				}
			}
		}
	}


}

void InfiniteWorldSystem::postUpdate(float dt) {

	int create_count = 0;
	int delete_count = 0;
	while (!createRequests_.empty()) {
		if (create_count < 2) {
			ENGINE.sendEvent<TerrainChunkRequest>(createRequests_.front());
			createRequests_.pop();
			create_count++;
		}
		else {
			break;
		}
	}

	while (!deleteRequests_.empty()) {
		if (delete_count < 2) {
			ENGINE.sendEvent<TerrainChunkDestroyedEvent>(deleteRequests_.front());
			deleteRequests_.pop();
			delete_count++;
		}
		else {
			break;
		}
	}
}

void InfiniteWorldSystem::onCameraCreated(const CameraCreated * e) {
	if (activeCamera_ == nullptr) {
		activeCamera_ = dynamic_cast<IGameCamera*>(ENGINE.getEntityManager().getEntity<GameCamera>(e->id_));
		assert(activeCamera_ != nullptr && "Falied to get IGameCamera pointer.");
	}
	else {
		assert(0 && "There should bo only one active camera");
	}
}

void InfiniteWorldSystem::onCameraDestroyed(const CameraDestroyed * e) {
	activeCamera_ = nullptr;

}



XMINT3 InfiniteWorldSystem::hashWorldCoord(XMINT3 coord) {
	return XMINT3(
			((coord.x - originCoord_.x) / size_) + 1, 
			((coord.y - originCoord_.y) / size_) + 1, 
			((coord.z - originCoord_.z) / size_) + 1
			);
}
