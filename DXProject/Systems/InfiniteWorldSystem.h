#pragma once
#include "../ECS/System.h"
#include "../ECS/Event/IEventListener.h"
#include "../Utilities/Structures/Array3D.h"
#include "../Entities/IGameCamera.h"
#include "../Events/CameraCreated.h"
#include "../Events/CameraDestroyed.h"
#include "../Entities/TerrainChunk.h"
#include "../Events/VoxelDataGeneratedEvent.h"

#include <memory>
#include <vector>
#include <queue>

class InfiniteWorldSystem : public System<InfiniteWorldSystem>, public IEventListener {
public:
	InfiniteWorldSystem();
	~InfiniteWorldSystem();

	virtual void preUpdate(float dt) override;

	virtual void update(float dt) override;

	virtual void postUpdate(float dt) override;

private:
	void onCameraCreated(const CameraCreated * e);
	void onCameraDestroyed(const CameraDestroyed * e);
	void onVoxelDataGeneratedEvent(const VoxelDataGeneratedEvent * e);

	XMINT3 hashWorldCoord(XMINT3 coord);

	Array3D<std::vector<EntityId>, 3, 3, 3> activeWorldChunks_;
	IGameCamera * activeCamera_;
	int size_ = 4;
	std::vector<EntityId> terrainChunksGenerated_;
	XMFLOAT3 originPosition_;
	XMINT3 originCoord_;
	bool initialGeneration_;

	std::queue<XMINT3> createRequests_;
	std::queue<XMINT3> deleteRequests_;

};