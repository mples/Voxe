#pragma once

#include "../Utilities/Singleton.h"
#include "StageFactory.h"

class StageManager : public Singleton<StageManager> {
public:
	StageManager();
	~StageManager();

	void addStage(StageType type, StageBuilder * builder);
	void removeStage(StageType type);

	void setStartingStage(StageType type);
	void setNextStage(StageType type);

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

	StageType currentStage_;
	StageType nextStage_;
	StageFactory stageFactory_;
	Stage * currentStagePointer_;
};

#define STAGE_MANAGER StageManager::getInstance()