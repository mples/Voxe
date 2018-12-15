#pragma once
#include "Stage.h"
#include "Player.h"
#include "../../World.h"

class MyStage : public Stage {
public:
	MyStage();
	~MyStage();

	// Inherited via Stage
	virtual void init() override;
	virtual void update(float dt) override;
	virtual void shutdown() override;

	Player* player_;
	Object* object_;
	World world_;
};

