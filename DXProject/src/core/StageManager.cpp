#include "StageManager.h"
#include "ObjectManager.h"
#include "GraphicEngine.h"
#include "Input.h"

StageManager::StageManager() : isQuitting_(false), isRestarting_(false), currentStage_(nullptr), nextStage_(nullptr), isChanging_(false), Singleton<StageManager>() {
}


StageManager::~StageManager() {
}

void StageManager::setStartingStage(Stage * stage) {
	currentStage_ = stage;
	nextStage_ = stage;
}

void StageManager::setNextStage(Stage * stage) {
	nextStage_ = stage;
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

	initStage();

	while (!isChanging_ && !isRestarting_ && !isQuitting_) {
		Input::getInstance().processInput();
		ObjectManager::getInstance().update(dt);
		GraphicEngine::getInstance().draw();
		currentStage_->update(dt);
	}

	changeStage();
}

void StageManager::initStage() {
	currentStage_->init();

	isRestarting_ = false;

	isChanging_ = false;

}

void StageManager::changeStage() {
	if (!isRestarting_) {
		currentStage_->shutdown();
		delete currentStage_;
	}
	else {
		currentStage_->shutdown();
	}
	currentStage_ = nextStage_;
}
