#pragma once
#include "Stage.h"

#include "Player.h"

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
};

