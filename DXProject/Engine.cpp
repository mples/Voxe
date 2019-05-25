#include "Engine.h"

#include "Systems/RenderSystem.h"
#include "Systems/TerrainGenerationSystem.h"
#include "Systems/TerrainMeshGeneratorSystem.h"
#include "Systems/VoxelDataGenerationSystem.h"
#include "Systems/FrustumCullingSystem.h"

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
	systemManager_.addSystem<TerrainMeshGenerationSystem>();
	systemManager_.addSystem<VoxelDataGenerationSystem>();
	systemManager_.addSystem<FrustumCullingSystem>();
}

bool Engine::processMessages() {
	return windowContainer_.renderWindow_.procesMessages();
}

void Engine::update() {
	double dt = timer_.getTimePassedMilisec();
	timer_.restart();

	char s[256];
	sprintf(s, "Dt: %f\n", dt);
	OutputDebugStringA(s);

	eventHandler_.dispatchEvents();
	systemManager_.update(dt);


	INPUT.processInput(dt);
	STAGE_MANAGER.update(dt);

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
