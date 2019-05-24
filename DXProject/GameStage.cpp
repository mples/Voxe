#include "GameStage.h"
#include "Engine.h"
#include "Events/TerrainChunkRequest.h"
#include "Entities/GameCamera.h"
#include "Events/CameraCreated.h"
#include <d3d11.h>

GameStage::GameStage() {
}


GameStage::~GameStage() {
}

void GameStage::init() {
	ENGINE.sendEvent<TerrainChunkRequest>(0, 0, 0);
	ENGINE.getEntityManager().createEntity<GameCamera>(90.0f, 800 / 800, 0.1f, 100.0f, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 5.0f, -5.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));
	//EntityId eid =ENGINE.getEntityManager().createEntity<GameCamera>();
	//ENGINE.getComponentManager().addComponent<CameraComponent>(eid, 90.0f, 800 / 800, 0.1f, 100.0f, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));
	//ENGINE.sendEvent<CameraCreated>(eid);
}

void GameStage::update(float dt) {
//	OutputDebugStringW(L"Game stage");
}

void GameStage::shutdown() {
}
