#pragma once
#include "../Stage/Stage.h"

class GameStage : public Stage {
public:
	GameStage();
	~GameStage();

	// Inherited via Stage
	virtual void init() override;
	virtual void update(float dt) override;
	virtual void shutdown() override;
private:

};

