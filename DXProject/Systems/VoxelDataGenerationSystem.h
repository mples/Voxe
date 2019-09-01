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

	void setGenerator(Generator * gen);
private:
	void onVoxelDataRequest(const VoxelDataRequest* e);

	Generator * generator_;
	std::list<EntityId> chunksToGenerateData_;

	unsigned int MAX_CHUNK_GENERATED_PER_UDPATE = 2;

};