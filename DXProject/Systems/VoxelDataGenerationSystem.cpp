#include "VoxelDataGenerationSystem.h"
#include "../Components/BlocksDataComponent.h"
#include "../Components/WorldCoordinateComponent.h"
#include "../Events/VoxelDataGeneratedEvent.h"
VoxelDataGenerationSystem::VoxelDataGenerationSystem() : noise_(1234, 4, 50.0, 0.2, -5), generator_(&noise_), IEventListener(ENGINE.getEventHandler()) { //TODO parameters
	std::function<void(const VoxelDataRequest*) > f = [&](const VoxelDataRequest* e) {
		onVoxelDataRequest(e);
	};
	registerEventCallback<VoxelDataRequest>(f);
}

VoxelDataGenerationSystem::~VoxelDataGenerationSystem() {
}

void VoxelDataGenerationSystem::update(float dt) {
	auto it = entitiesToUpdate_.begin();
	while (it != entitiesToUpdate_.end() ) {
		
		BlockType blocks[DIMENSION][DIMENSION][DIMENSION];
		ENGINE.getEntityManager().getEntity<TerrainChunk>(*it);
		WorldCoordinateComponent * coord_comp = ENGINE.getComponentManager().getComponent<WorldCoordinateComponent>(*it);
		if (coord_comp != nullptr) {
			generator_.generate(blocks, coord_comp->getCoord());
			ENGINE.getComponentManager().addComponent<BlocksDataComponent>(*it, blocks);
			ENGINE.sendEvent<VoxelDataGeneratedEvent>(*it);
		}

		it++;
	}
	entitiesToUpdate_.clear();
}


void VoxelDataGenerationSystem::onVoxelDataRequest(const VoxelDataRequest * e) {
	entitiesToUpdate_.push_back(e->id_);
}
