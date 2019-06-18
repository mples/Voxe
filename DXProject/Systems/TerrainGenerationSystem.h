#pragma once
#include "../ECS/System.h"
#include "../ECS/Event/IEventListener.h"
#include "../Events/TerrainChunkRequest.h"
#include "../Events/TerrainChunkDestroyedEvent.h"
#include "../Entities/TerrainChunk.h"
#include "../Voxel/TerrainCoord.h"
#include "../Events/VoxelChangeRequest.h"

#include <DirectXMath.h>
#include <unordered_map>
#include <list>

using namespace DirectX;

class TerrainGenerationSystem : public System<TerrainGenerationSystem>, public IEventListener {
	struct VoxelChangeData {
		VoxelChangeData(XMINT3 chunk_coord, XMINT3 voxel_coord, BlockType type) : chunkCoord_(chunk_coord), voxelCoord_(voxel_coord), type_(type) {}

		XMINT3 chunkCoord_;
		XMINT3 voxelCoord_;
		BlockType type_;
	};

public:
	TerrainGenerationSystem();
	~TerrainGenerationSystem();

	virtual void preUpdate(float dt) override;

	virtual void update(float dt) override;

	virtual void postUpdate(float dt) override;

	EntityId getTerrainChunk(XMINT3 coord);
	EntityId getTerrainChunk(int x, int y, int z);

private:
	void insertTerrainNeightbours(EntityId id, XMINT3 & coord);

	void eraseAsNeightbour(EntityId id);

	void onTerrainChunkRequest(const TerrainChunkRequest* e);

	void onTerrainChunkDestroyedEvent(const TerrainChunkDestroyedEvent * e);

	void onVoxelChangeRequest(const VoxelChangeRequest * e);

	std::list<XMINT3> entitiesToGenerate_;
	std::list<XMINT3> entitiesToDestroy_;
	std::list<VoxelChangeData> voxelChangeRequests_;
	std::unordered_map<TerrainCoord, EntityId> activeTerrainChunks_;

	int MAX_CHUNK_TO_GENERATE = 10;
};
