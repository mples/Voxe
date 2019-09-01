#pragma once
#include "../ECS/Component.h"
#include "../ECS/IEntity.h"

class TerrainNeighboursComponent : public Component<TerrainNeighboursComponent> {
public:
	TerrainNeighboursComponent(EntityId l, EntityId r, EntityId t, EntityId bottom, EntityId f, EntityId back);
	~TerrainNeighboursComponent();

	EntityId left_;
	EntityId right_;
	EntityId top_;
	EntityId bottom_;
	EntityId front_;
	EntityId back_;
private:
};