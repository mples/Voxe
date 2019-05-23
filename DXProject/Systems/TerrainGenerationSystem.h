#pragma once
#include "../ECS/System.h"
#include "../ECS/Event/IEventListener.h"
#include "../Events/TerrainChunkRequest.h"
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

	void insertTerrainNeightbours(EntityId id, XMINT3 & coord);

	void onTerrainChunkRequest(const TerrainChunkRequest* e);

private:
	std::list<XMINT3> entitiesToGenerate_;
	std::unordered_map<TerrainCoord, EntityId> activeTerrainChunks_;
};
