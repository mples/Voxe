#pragma once
#include "../ECS/Component.h"
#include "../ECS/IEntity.h"

class TerrainNeightboursComponent : public Component<TerrainNeightboursComponent> {
public:
	TerrainNeightboursComponent(EntityId l, EntityId r, EntityId t, EntityId bottom, EntityId f, EntityId back);
	~TerrainNeightboursComponent();

	EntityId left_;
	EntityId right_;
	EntityId top_;
	EntityId bottom_;
	EntityId front_;
	EntityId back_;
private:
};