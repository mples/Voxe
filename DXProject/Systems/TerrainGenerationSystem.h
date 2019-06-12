#pragma once
#include "../ECS/System.h"
#include "../ECS/Event/IEventListener.h"
#include "../Events/TerrainChunkRequest.h"
#include "../Events/TerrainChunkDestroyedEvent.h"
#include "../Entities/TerrainChunk.h"
#include "../Voxel/TerrainCoord.h"

#include <DirectXMath.h>
#include <unordered_map>
#include <list>

using namespace DirectX;

class TerrainGenerationSystem : public System<TerrainGenerationSystem>, public IEventListener {
public:
	TerrainGenerationSystem();
	~TerrainGenerationSystem();

	virtual void preUpdate(float dt) override;

	virtual void update(float dt) override;

	virtual void postUpdate(float dt) override;



private:
	void insertTerrainNeightbours(EntityId id, XMINT3 & coord);

	void eraseAsNeightbour(EntityId id);

	void onTerrainChunkRequest(const TerrainChunkRequest* e);

	void onTerrainChunkDestroyedEvent(const TerrainChunkDestroyedEvent * e);

	std::list<XMINT3> entitiesToGenerate_;
	std::list<XMINT3> entitiesToDestroy_;
	std::unordered_map<TerrainCoord, EntityId> activeTerrainChunks_;

	int MAX_CHUNK_TO_GENERATE = 10;
};
