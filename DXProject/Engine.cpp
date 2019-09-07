#include "Engine.h"

#include "Systems/RenderSystem.h"
#include "Systems/TerrainGenerationSystem.h"
#include "Systems/TerrainMeshGeneratorSystem.h"
#include "Systems/VoxelDataGenerationSystem.h"
#include "Systems/FrustumCullingSystem.h"
#include "Systems/OcclusionCullingSystem.h"
#include "Systems/InfiniteWorldSystem.h"

#include "Utilities/JobSystem.h"

Engine::Engine() : entityManager_(&componentManager_) {
}


Engine::~Engine() {
	MouseManager::removeInstance();
	KeyboardManager::removeInstance();
	InputManager::removeInstance();
	StageManager::removeInstance();
	JobSystem::removeInstance();
}

bool Engine::init(HINSTANCE hInstance, std::wstring window_title, std::wstring window_class, int width, int height) {
	if (!windowContainer_.renderWindow_.initialize(&windowContainer_, hInstance, window_title, window_class, width, height)) {
		return false;
	}

	timer_.start();
	globalTimer_.start();
	fpsTimer_.start();

	MouseManager::createInstance();
	KeyboardManager::createInstance();
	InputManager::createInstance();
	StageManager::createInstance();
	JobSystem::createInstance();

	initializeSystems(windowContainer_.renderWindow_.getHWND(), width, height);

	return true;
}

void Engine::initializeSystems(HWND hwd, int width, int height) {
	systemManager_.addSystem<RenderSystem>(hwd, width, height);
	systemManager_.addSystem<TerrainGenerationSystem>();
	systemManager_.setSystemUpdateInterval<TerrainGenerationSystem>(66.66f);

	systemManager_.addSystem<TerrainMeshGenerationSystem>();
	systemManager_.setSystemUpdateInterval<TerrainMeshGenerationSystem>(66.66f);

	systemManager_.addSystem<VoxelDataGenerationSystem>();
	systemManager_.setSystemUpdateInterval<VoxelDataGenerationSystem>(66.66f);


	systemManager_.addSystem<FrustumCullingSystem>();
	systemManager_.setSystemUpdateInterval<FrustumCullingSystem>(33.33f);

	systemManager_.addSystem<OcclusionCullingSystem>();
	systemManager_.setSystemUpdateInterval<OcclusionCullingSystem>(33.33f);

	systemManager_.addSystem<InfiniteWorldSystem>();
	systemManager_.setSystemUpdateInterval<InfiniteWorldSystem>(33.33f);

}

bool Engine::processMessages() {
	return windowContainer_.renderWindow_.procesMessages();
}

void Engine::update() {
	double dt = timer_.getTimePassedMilisec();
	timer_.restart();

	static int fps_counter = 0;
	static char s[256];
	fps_counter++;


	systemManager_.update(dt);
	eventHandler_.dispatchEvents();

	INPUT.processInput(dt);
	STAGE_MANAGER.update(dt);

	if (fpsTimer_.getTimePassedMilisec() > 1000.0) {
		sprintf(s, "FPS: %u\n", fps_counter);
		fps_counter = 0;
		fpsTimer_.restart();
	}
	OutputDebugStringA(s);

}

EventHandler * Engine::getEventHandler() {
	return &eventHandler_;
}

ComponentManager & Engine::getComponentManager() {
	return componentManager_;
}

SystemManager & Engine::getSystemManager() {
	return systemManager_;
}

EntityManager & Engine::getEntityManager() {
	return entityManager_;
}

double Engine::getGlobalTime() {
	return globalTimer_.getTimePassedMilisec();
}
