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

	int generated_count = 0;

	while (!chunksToGenerateData_.empty()) {
		if (generated_count == MAX_CHUNK_GENERATED_PER_UDPATE) {
			break;
		}
		EntityId & e_id = chunksToGenerateData_.front();

		Array3D<BlockType, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> blocks(BlockType::AIR);
		WorldCoordinateComponent * coord_comp = ENGINE.getComponentManager().getComponentByEntityId<WorldCoordinateComponent>(e_id);
		if (coord_comp != nullptr) {
			generator_.generate(blocks, coord_comp->getCoord());
			ENGINE.getComponentManager().addComponent<BlocksDataComponent>(e_id, blocks);
			ENGINE.sendEvent<VoxelDataGeneratedEvent>(e_id);

			chunksToGenerateData_.pop_front();
			generated_count++;
		}
		else {
			break;
		}

	}

}


void VoxelDataGenerationSystem::onVoxelDataRequest(const VoxelDataRequest * e) {
	chunksToGenerateData_.push_back(e->id_);
}
