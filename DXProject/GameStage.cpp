#include "GameStage.h"



GameStage::GameStage() : noise_(1234, 4, 50.0, 0.1, -5), generator_(&noise_), world_(&generator_) {
}


GameStage::~GameStage() {
}

void GameStage::init() {
	
}

void GameStage::update(float dt) {
//	OutputDebugStringW(L"Game stage");
}

void GameStage::shutdown() {
}
