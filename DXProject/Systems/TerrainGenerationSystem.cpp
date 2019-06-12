#include "TerrainGenerationSystem.h"

#include "../Engine.h"

#include "../Components/TransformComponent.h"
#include "../Components/MeshComponent.h"
#include "../Components/MaterialComponent.h"
#include "../Components/BlocksDataComponent.h"
#include "../Components/WorldCoordinateComponent.h"
#include "../Components/TerrainNeighboursComponent.h"

#include "../Events/VoxelDataRequest.h"
#include "../Events/TerrainChunkChanged.h"

TerrainGenerationSystem::TerrainGenerationSystem() : IEventListener(ENGINE.getEventHandler()) {
	registerEventCallback<TerrainChunkRequest>([&](const TerrainChunkRequest* e) {
		onTerrainChunkRequest(e);
	});

	registerEventCallback<TerrainChunkDestroyedEvent>([&](const TerrainChunkDestroyedEvent * e) {
		onTerrainChunkDestroyedEvent(e);
	});
}

TerrainGenerationSystem::~TerrainGenerationSystem() {
}

void TerrainGenerationSystem::preUpdate(float dt) {
}

void TerrainGenerationSystem::update(float dt) {
	int generated_chunks = 0;

	while (!entitiesToGenerate_.empty()) {
		if (generated_chunks == MAX_CHUNK_TO_GENERATE) {
			break;
		}
		XMINT3 coord = entitiesToGenerate_.front();

		EntityId id = ENGINE.getEntityManager().createEntity<TerrainChunk>(coord);

		insertTerrainNeightbours(id, coord);

		activeTerrainChunks_[TerrainCoord(coord)] = id;

		ENGINE.sendEvent<VoxelDataRequest>(id);

		entitiesToGenerate_.pop_front();
		generated_chunks++;
	}

	while(!entitiesToDestroy_.empty()) {
		if (generated_chunks == MAX_CHUNK_TO_GENERATE) {
			break;
		}
		XMINT3 coord = entitiesToDestroy_.front();

		auto found = activeTerrainChunks_.find(TerrainCoord(coord));
		if (found != activeTerrainChunks_.end()) {
			eraseAsNeightbour(found->second);
			ENGINE.getEntityManager().eraseEntity<TerrainChunk>(found->second);
			activeTerrainChunks_.erase(found);
		}

		entitiesToDestroy_.pop_front();
		generated_chunks++;
	}
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
		ENGINE.sendEvent< TerrainChunkChanged>(left->second);
	}

	auto right = activeTerrainChunks_.find(TerrainCoord(coord.x + 1, coord.y, coord.z));
	if (right != activeTerrainChunks_.end()) {
		right_eid = right->second;
		TerrainNeightboursComponent* right_comp = ENGINE.getComponentManager().getComponentByEntityId<TerrainNeightboursComponent>(right->second);
		right_comp->left_ = id;
		ENGINE.sendEvent< TerrainChunkChanged>(right->second);

	}

	auto top = activeTerrainChunks_.find(TerrainCoord(coord.x, coord.y + 1, coord.z));
	if (top != activeTerrainChunks_.end()) {
		top_eid = top->second;
		TerrainNeightboursComponent* top_comp = ENGINE.getComponentManager().getComponentByEntityId<TerrainNeightboursComponent>(top->second);
		top_comp->bottom_ = id;
		ENGINE.sendEvent< TerrainChunkChanged>(top->second);

	}

	auto bottom = activeTerrainChunks_.find(TerrainCoord(coord.x, coord.y - 1, coord.z));
	if (bottom != activeTerrainChunks_.end()) {
		bottom_eid = bottom->second;
		TerrainNeightboursComponent* bottom_comp = ENGINE.getComponentManager().getComponentByEntityId<TerrainNeightboursComponent>(bottom->second);
		bottom_comp->top_ = id;
		ENGINE.sendEvent< TerrainChunkChanged>(bottom->second);

	}

	auto front = activeTerrainChunks_.find(TerrainCoord(coord.x, coord.y, coord.z + 1));
	if (front != activeTerrainChunks_.end()) {
		front_eid = front->second;
		TerrainNeightboursComponent* front_comp = ENGINE.getComponentManager().getComponentByEntityId<TerrainNeightboursComponent>(front->second);
		front_comp->back_ = id;
		ENGINE.sendEvent< TerrainChunkChanged>(front->second);

	}

	auto back = activeTerrainChunks_.find(TerrainCoord(coord.x, coord.y, coord.z - 1));
	if (back != activeTerrainChunks_.end()) {
		back_eid = back->second;
		TerrainNeightboursComponent* back_comp = ENGINE.getComponentManager().getComponentByEntityId<TerrainNeightboursComponent>(back->second);
		back_comp->front_ = id;
		ENGINE.sendEvent< TerrainChunkChanged>(back->second);

	}

	ENGINE.getComponentManager().addComponent<TerrainNeightboursComponent>(id, left_eid, right_eid, top_eid, bottom_eid, front_eid, back_eid);

}

void TerrainGenerationSystem::eraseAsNeightbour(EntityId id) {
	TerrainNeightboursComponent * current_neight = ENGINE.getComponentManager().getComponentByEntityId<TerrainNeightboursComponent>(id);
	if (current_neight->left_ != EntityId::getInvalidHandle()) {
		TerrainNeightboursComponent * left_neight = ENGINE.getComponentManager().getComponentByEntityId<TerrainNeightboursComponent>(current_neight->left_);
		left_neight->right_ = EntityId::getInvalidHandle();
		ENGINE.sendEvent<TerrainChunkChanged>(current_neight->left_);
	}
	if (current_neight->right_ != EntityId::getInvalidHandle()) {
		TerrainNeightboursComponent * right_neight = ENGINE.getComponentManager().getComponentByEntityId<TerrainNeightboursComponent>(current_neight->right_);
		right_neight->left_ = EntityId::getInvalidHandle();
		ENGINE.sendEvent<TerrainChunkChanged>(current_neight->right_);
	}
	if (current_neight->top_ != EntityId::getInvalidHandle()) {
		TerrainNeightboursComponent * top_neight = ENGINE.getComponentManager().getComponentByEntityId<TerrainNeightboursComponent>(current_neight->top_);
		top_neight->bottom_ = EntityId::getInvalidHandle();
		ENGINE.sendEvent<TerrainChunkChanged>(current_neight->top_);
	}
	if (current_neight->bottom_ != EntityId::getInvalidHandle()) {
		TerrainNeightboursComponent * bottom_neight = ENGINE.getComponentManager().getComponentByEntityId<TerrainNeightboursComponent>(current_neight->bottom_);
		bottom_neight->top_ = EntityId::getInvalidHandle();
		ENGINE.sendEvent<TerrainChunkChanged>(current_neight->bottom_);
	}
	if (current_neight->front_ != EntityId::getInvalidHandle()) {
		TerrainNeightboursComponent * front_neight = ENGINE.getComponentManager().getComponentByEntityId<TerrainNeightboursComponent>(current_neight->front_);
		front_neight->back_ = EntityId::getInvalidHandle();
		ENGINE.sendEvent<TerrainChunkChanged>(current_neight->front_);
	}
	if (current_neight->back_ != EntityId::getInvalidHandle()) {
		TerrainNeightboursComponent * back_neight = ENGINE.getComponentManager().getComponentByEntityId<TerrainNeightboursComponent>(current_neight->back_);
		back_neight->front_ = EntityId::getInvalidHandle();
		ENGINE.sendEvent<TerrainChunkChanged>(current_neight->back_);
	}
}

void TerrainGenerationSystem::onTerrainChunkRequest(const TerrainChunkRequest * e) {
	entitiesToGenerate_.push_back(e->coord_);
}

void TerrainGenerationSystem::onTerrainChunkDestroyedEvent(const TerrainChunkDestroyedEvent * e) {
	entitiesToDestroy_.push_back(e->coord_);
}
