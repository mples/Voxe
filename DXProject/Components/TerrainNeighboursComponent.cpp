#include "TerrainNeighboursComponent.h"
#include "../Engine.h"
#include "../Events/TerrainChunkChanged.h"

TerrainNeighboursComponent::TerrainNeighboursComponent(EntityId l, EntityId r, EntityId t, EntityId bottom, EntityId f, EntityId back) 
: left_(l),
right_(r),
top_(t),
bottom_(bottom),
front_(f),
back_(back)
{
}

TerrainNeighboursComponent::~TerrainNeighboursComponent() {
}
