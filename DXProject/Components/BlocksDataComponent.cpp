#include "BlocksDataComponent.h"

#include "../Engine.h"
#include "../Events/TerrainChunkChanged.h"
#include "../Components/TerrainNeighboursComponent.h"

BlocksDataComponent::BlocksDataComponent() : isEmpty_(true) {
	blocks_.fillWith(BlockType::AIR);
}

BlocksDataComponent::BlocksDataComponent(Array3D<BlockType, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> &b) : isEmpty_(true) {
	blocks_ = b;
	for (int x = 0; x < TERRAIN_CHUNK_DIM; x++) {
		for (int y = 0; y < TERRAIN_CHUNK_DIM; y++) {
			for (int z = 0; z < TERRAIN_CHUNK_DIM; z++) {
				if (blocks_.at(x, y, z) != BlockType::AIR) {
					isEmpty_ = false;
					return;
				}
			}
		}
	}
}

BlocksDataComponent::~BlocksDataComponent() {
}

void BlocksDataComponent::setBlocks(Array3D<BlockType, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM, TERRAIN_CHUNK_DIM> &b) {
	blocks_ = b;
	isEmpty_ = true;

	for (int x = 0; x < TERRAIN_CHUNK_DIM; x++) {
		for (int y = 0; y < TERRAIN_CHUNK_DIM; y++) {
			for (int z = 0; z < TERRAIN_CHUNK_DIM; z++) {
				if (blocks_.at(x,y,z) != BlockType::AIR) {
					isEmpty_ = false;
					return;
				}
			}
		}
	}
}

BlockType BlocksDataComponent::getBlock(UINT x, UINT y, UINT z) {
	return blocks_.at(x, y, z);
}

BlockType BlocksDataComponent::getBlock(XMINT3 coord) {
	return blocks_.at(coord.x, coord.y, coord.z);
}

void BlocksDataComponent::setBlock(XMINT3 coord, BlockType type) {
	if (getBlock(coord) != type) {
		if(coord.x == 0) {
			TerrainNeightboursComponent * neightbours = ENGINE.getComponentManager().getComponentByEntityId<TerrainNeightboursComponent>(getOwner());
			if (neightbours && neightbours->left_ != ComponentId::getInvalidHandle()) {
				ENGINE.sendEvent<TerrainChunkChanged>(neightbours->left_);
			}
		}
		else if (coord.x == TERRAIN_CHUNK_DIM - 1) {
			TerrainNeightboursComponent * neightbours = ENGINE.getComponentManager().getComponentByEntityId<TerrainNeightboursComponent>(getOwner());
			if (neightbours && neightbours->right_ != ComponentId::getInvalidHandle()) {
				ENGINE.sendEvent<TerrainChunkChanged>(neightbours->right_);
			}
		}
		if (coord.y == 0) {
			TerrainNeightboursComponent * neightbours = ENGINE.getComponentManager().getComponentByEntityId<TerrainNeightboursComponent>(getOwner());
			if (neightbours && neightbours->bottom_ != ComponentId::getInvalidHandle()) {
				ENGINE.sendEvent<TerrainChunkChanged>(neightbours->bottom_);
			}
		}
		else if (coord.y == TERRAIN_CHUNK_DIM - 1) {
			TerrainNeightboursComponent * neightbours = ENGINE.getComponentManager().getComponentByEntityId<TerrainNeightboursComponent>(getOwner());
			if (neightbours && neightbours->top_ != ComponentId::getInvalidHandle()) {
				ENGINE.sendEvent<TerrainChunkChanged>(neightbours->top_);
			}
		}
		if (coord.z == 0) {
			TerrainNeightboursComponent * neightbours = ENGINE.getComponentManager().getComponentByEntityId<TerrainNeightboursComponent>(getOwner());
			if (neightbours && neightbours->back_ != ComponentId::getInvalidHandle()) {
				ENGINE.sendEvent<TerrainChunkChanged>(neightbours->back_);
			}
		}
		else if (coord.z == TERRAIN_CHUNK_DIM - 1) {
			TerrainNeightboursComponent * neightbours = ENGINE.getComponentManager().getComponentByEntityId<TerrainNeightboursComponent>(getOwner());
			if (neightbours && neightbours->front_ != ComponentId::getInvalidHandle()) {
				ENGINE.sendEvent<TerrainChunkChanged>(neightbours->front_);
			}
		}

		blocks_.at(coord.x, coord.y, coord.z) = type;
		ENGINE.sendEvent<TerrainChunkChanged>(getOwner());
	}
}

void BlocksDataComponent::setBlock(UINT x, UINT y, UINT z, BlockType type) {
	if (getBlock(x,y,z) != type) {
		blocks_.at(x, y, z) = type;
		ENGINE.sendEvent<TerrainChunkChanged>(getOwner());
	}
}

bool BlocksDataComponent::isEmpty() {
	return isEmpty_;
}
