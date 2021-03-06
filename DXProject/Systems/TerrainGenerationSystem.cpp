#include "TerrainGenerationSystem.h"

#include "../Engine.h"

#include "../Components/TransformComponent.h"
#include "../Components/MeshComponent.h"
#include "../Components/MaterialComponent.h"
#include "../Components/BlocksDataComponent.h"
#include "../Components/WorldCoordinateComponent.h"
#include "../Components/TerrainNeighboursComponent.h"
#include "../Events/TerrainChunkChanged.h"

#include "../Events/VoxelDataRequest.h"

TerrainGenerationSystem::TerrainGenerationSystem() :  IEventListener(ENGINE.getEventHandler()) { //TODO parametize 
	std::function<void(const TerrainChunkRequest*) > f = [&](const TerrainChunkRequest* e) {
		onTerrainChunkRequest(e);
	};
	registerEventCallback<TerrainChunkRequest>(f);
}

TerrainGenerationSystem::~TerrainGenerationSystem() {
}

void TerrainGenerationSystem::preUpdate(float dt) {
}

void TerrainGenerationSystem::update(float dt) {
	auto it = entitiesToGenerate_.begin();
	while (it != entitiesToGenerate_.end()) {
		EntityId id = ENGINE.getEntityManager().createEntity<TerrainChunk>();

		ENGINE.getComponentManager().addComponent<WorldCoordinateComponent>(id, *it);

		insertTerrainNeightbours(id, *it);

		activeTerrainChunks_[TerrainCoord(*it)] = id;

		ENGINE.sendEvent<VoxelDataRequest>(id);

		it++;
	}

	entitiesToGenerate_.clear();
}

void TerrainGenerationSystem::postUpdate(float dt) {
}

void TerrainGenerationSystem::insertTerrainNeightbours(EntityId id, XMINT3 & coord) {
	EntityId left_eid = IEntity::getInvalidId();
	EntityId right_eid = IEntity::getInvalidId();
	EntityId top_eid = IEntity::getInvalidId();
	EntityId bottom_eid = IEntity::getInvalidId();
	EntityId front_eid = IEntity::getInvalidId();
	EntityId back_eid = IEntity::getInvalidId();

	auto left = activeTerrainChunks_.find(TerrainCoord(coord.x - 1, coord.y, coord.z));
	if (left != activeTerrainChunks_.end()) {
		left_eid = left->second;
		TerrainNeightboursComponent* left_comp = ENGINE.getComponentManager().getComponentByEntityId<TerrainNeightboursComponent>(left->second);
		left_comp->right_ = id;
		ENGINE.sendEvent<TerrainChunkChanged>(left->second);
	}

	auto right = activeTerrainChunks_.find(TerrainCoord(coord.x + 1, coord.y, coord.z));
	if (right != activeTerrainChunks_.end()) {
		right_eid = right->second;
		TerrainNeightboursComponent* right_comp = ENGINE.getComponentManager().getComponentByEntityId<TerrainNeightboursComponent>(right->second);
		right_comp->left_ = id;
		ENGINE.sendEvent<TerrainChunkChanged>(right->second);
	}

	auto top = activeTerrainChunks_.find(TerrainCoord(coord.x, coord.y + 1, coord.z));
	if (top != activeTerrainChunks_.end()) {
		top_eid = top->second;
		TerrainNeightboursComponent* top_comp = ENGINE.getComponentManager().getComponentByEntityId<TerrainNeightboursComponent>(top->second);
		top_comp->bottom_ = id;
		ENGINE.sendEvent<TerrainChunkChanged>(top->second);
	}

	auto bottom = activeTerrainChunks_.find(TerrainCoord(coord.x, coord.y - 1, coord.z));
	if (bottom != activeTerrainChunks_.end()) {
		bottom_eid = bottom->second;
		TerrainNeightboursComponent* bottom_comp = ENGINE.getComponentManager().getComponentByEntityId<TerrainNeightboursComponent>(bottom->second);
		bottom_comp->top_ = id;
		ENGINE.sendEvent<TerrainChunkChanged>(bottom->second);
	}

	auto front = activeTerrainChunks_.find(TerrainCoord(coord.x, coord.y, coord.z + 1));
	if (front != activeTerrainChunks_.end()) {
		front_eid = front->second;
		TerrainNeightboursComponent* front_comp = ENGINE.getComponentManager().getComponentByEntityId<TerrainNeightboursComponent>(front->second);
		front_comp->back_ = id;
		ENGINE.sendEvent<TerrainChunkChanged>(front->second);
	}

	auto back = activeTerrainChunks_.find(TerrainCoord(coord.x, coord.y, coord.z - 1));
	if (back != activeTerrainChunks_.end()) {
		back_eid = back->second;
		TerrainNeightboursComponent* back_comp = ENGINE.getComponentManager().getComponentByEntityId<TerrainNeightboursComponent>(back->second);
		back_comp->front_ = id;
		ENGINE.sendEvent<TerrainChunkChanged>(back->second);
	}

	ENGINE.getComponentManager().addComponent<TerrainNeightboursComponent>(id, left_eid, right_eid, top_eid, bottom_eid, front_eid, back_eid);

}

void TerrainGenerationSystem::onTerrainChunkRequest(const TerrainChunkRequest * e) {
	entitiesToGenerate_.push_back(e->coord_);
}
