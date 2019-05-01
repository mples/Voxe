#pragma once
#include "Stage\Stage.h"
#include "Voxel\World.h"
#include "Generation/SimpleGenerator.h"
#include "Generation/PerlinNoise.h"

class GameStage : public Stage{
public:
	GameStage();
	~GameStage();

	// Inherited via Stage
	virtual void init() override;
	virtual void update(float dt) override;
	virtual void shutdown() override;
private:
	World world_;
	SimpleGenerator generator_;
	PerlinNoise noise_;
};

