#include "TerrainChunk.h"
#include "../Engine.h"

TerrainChunk::TerrainChunk(XMINT3 coord) : coordComp_(nullptr) {
	coordComp_ = ENGINE.getComponentManager().addComponent<WorldCoordinateComponent>(id_, coord);
}

TerrainChunk::~TerrainChunk() {

}
