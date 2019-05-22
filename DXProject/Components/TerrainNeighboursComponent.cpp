#include "TerrainNeighboursComponent.h"

TerrainNeightboursComponent::TerrainNeightboursComponent(EntityId l, EntityId r, EntityId t, EntityId bottom, EntityId f, EntityId back) 
: left_(l),
right_(r),
top_(t),
bottom_(bottom),
front_(f),
back_(back_)
{
}

TerrainNeightboursComponent::~TerrainNeightboursComponent() {
}
