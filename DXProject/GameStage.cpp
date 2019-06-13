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
	/*for (int x = -5; x < 5; x++) {
		for (int y = -2; y < -1; y++) {
			for (int z = -5; z < 5; z++) {
				ENGINE.sendEvent<TerrainChunkRequest>(x, y, z);
			}
		}
	}*/

	ENGINE.getEntityManager().createEntity<GameCamera>(90.0f, 800 / 800, 0.1f, 64.0f, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 5.0f, -5.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));


}

void GameStage::update(float dt) {
}

void GameStage::shutdown() {
}
