#pragma once

#include "Singleton.h"
#include "Stage.h"

class StageManager : public Singleton<StageManager> {
public:
	StageManager();
	~StageManager();

	void setStartingStage(Stage* stage);
	void setNextStage(Stage* stage);

	void restartStage();
	void quitGame();
	bool isQuitting();
	bool isRestarting();

	void update(float dt);
private:
	void initStage();
	void changeStage();

	bool isRestarting_;
	bool isQuitting_;
	bool isChanging_;

	Stage* currentStage_;
	Stage* nextStage_;
};

