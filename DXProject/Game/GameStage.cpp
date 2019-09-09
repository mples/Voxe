#include "GameStage.h"
#include <d3d11.h>

#include "../Engine.h"
#include "../Events/TerrainChunkRequest.h"
#include "../Entities/GameCamera.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/VoxelDataGenerationSystem.h"

#include "../Events/CameraCreated.h"
#include "../Generation/SimpleGenerator.h"
#include "../Events/SetPointLightRequest.h"
#include "../Events/FogChangeRequest.h"


GameStage::GameStage() {
}


GameStage::~GameStage() {
}

void GameStage::init() {
	std::vector<BlockType> blocks_type({ BlockType::GRASS, BlockType::GRASS_DIRT });
	RenderSystem * render_sys = ENGINE.getSystemManager().getSystem<RenderSystem>();
	render_sys->createTextureAtlas(L"Data/Textures/texture_atlas.jpg", blocks_type);

	Noise * noise = new PerlinNoise(1234, 4, 50.0, 0.2, -5);
	Generator * gen = new SimpleGenerator(noise);
	VoxelDataGenerationSystem * gen_sys = ENGINE.getSystemManager().getSystem<VoxelDataGenerationSystem>();
	gen_sys->setGenerator(gen);

	ENGINE.sendEvent<FogChangeRequest>(DirectX::XMFLOAT3(0.5f, 0.25f, 1.0f), 52.0f, 96.0f);

	ENGINE.getEntityManager().createEntity<GameCamera>(90.0f, 800 / 800, 0.1f, 96.0f, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 5.0f, -5.0f), XMFLOAT3(0.0f, 0.0f, 0.0f));

}

void GameStage::update(float dt) {
}

void GameStage::shutdown() {
}