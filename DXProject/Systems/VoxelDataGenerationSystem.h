#pragma once
#include "../Engine.h"
#include "../ECS/System.h"
#include "../Generation/SimpleGenerator.h"
#include "../Generation/PerlinNoise.h"
#include "../ECS/Event/IEventListener.h"
#include "../Events/VoxelDataRequest.h"
#include "../Entities/TerrainChunk.h"
#include <list>

class VoxelDataGenerationSystem : public System<VoxelDataGenerationSystem>, public IEventListener {
public:
	VoxelDataGenerationSystem();
	~VoxelDataGenerationSystem();

	virtual void update(float dt) override;
private:
	void onVoxelDataRequest(const VoxelDataRequest* e);

	SimpleGenerator generator_;
	PerlinNoise noise_;
	std::list<EntityId> entitiesToUpdate_;
};