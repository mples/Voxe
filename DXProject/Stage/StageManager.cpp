#include "StageManager.h"


StageManager::StageManager() : isQuitting_(false), isRestarting_(false), currentStage_(StageType::INVALID), currentStagePointer_(nullptr), nextStage_(StageType::INVALID), isChanging_(false), Singleton<StageManager>() {
}

StageManager::~StageManager() {
}

void StageManager::addStage(StageType type, StageBuilder * builder) {
	stageFactory_.addBuilder(type, builder);
}

void StageManager::removeStage(StageType type) {
	stageFactory_.removeBuilder(type);
}

void StageManager::setStartingStage(StageType type) {
	currentStage_ = type;
	nextStage_ = type;
	isChanging_ = true;
	initStage();
}

void StageManager::setNextStage(StageType type) {
	nextStage_ = type;
	isChanging_ = true;
}

void StageManager::restartStage() {
	isRestarting_ = true;
}

void StageManager::quitGame() {
	isQuitting_ = true;
}

bool StageManager::isQuitting() {
	return isQuitting_;
}

bool StageManager::isRestarting() {
	return isRestarting_;
}

void StageManager::update(float dt) {
	if (currentStagePointer_ && !isChanging_ && !isRestarting_ && !isQuitting_) {
		currentStagePointer_->update(dt);
	}
	else {
		changeStage();
	}
}

void StageManager::initStage() {
	if (isRestarting_) {
		currentStagePointer_->init();
		isRestarting_ = false;
	}
	else if (isChanging_) {
		currentStagePointer_ = stageFactory_.build(currentStage_);
		currentStagePointer_->init();
		isChanging_ = false;
	}
}

void StageManager::changeStage() {
	if (currentStagePointer_) {
		if (!isRestarting_) {
			currentStagePointer_->shutdown();
			delete currentStagePointer_;
			currentStagePointer_ = nullptr;
		}
		else {
			currentStagePointer_->shutdown();
		}
	}
	currentStage_ = nextStage_;
	initStage();
}