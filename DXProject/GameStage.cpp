#include "GameStage.h"



GameStage::GameStage() : noise_(1234), generator_(&noise_), world_(&generator_) {
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
