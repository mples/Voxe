#include "Engine.h"

#include "Systems/RenderSystem.h"
#include "Systems/TerrainGenerationSystem.h"
#include "Systems/TerrainMeshGeneratorSystem.h"
#include "Systems/VoxelDataGenerationSystem.h"

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




	engineContext_.addActionMapping(Action::MOVE_FRONT, KeyboardEvent(KeyboardEvent::Type::PRESS, 'W'));
	engineContext_.addActionMapping(Action::MOVE_BACK, KeyboardEvent(KeyboardEvent::Type::PRESS, 'S'));
	engineContext_.addActionMapping(Action::MOVE_RIGHT, KeyboardEvent(KeyboardEvent::Type::PRESS, 'D'));
	engineContext_.addActionMapping(Action::MOVE_LEFT, KeyboardEvent(KeyboardEvent::Type::PRESS, 'A'));
	engineContext_.addActionMapping(Action::MOVE_DOWN, KeyboardEvent(KeyboardEvent::Type::PRESS, 'Z'));
	engineContext_.addActionMapping(Action::MOVE_UP, KeyboardEvent(KeyboardEvent::Type::PRESS, VK_SPACE));

	engineContext_.addRangeMapping(Range::LOOK_X, RawAxis(AxisType::RAW_INPUT_MOUSE_X));
	engineContext_.addRangeMapping(Range::LOOK_Y, RawAxis(AxisType::RAW_INPUT_MOUSE_Y));

	static float camera_speed = 0.05f;

	INPUT.addFrontContext(&engineContext_);
	InputCallback callback = [=](MappedInput& input) {
	/*	auto move_left = input.actions_.find(Action::MOVE_LEFT);
		if (move_left != input.actions_.end()) {
			GRAPHIC_ENGINE.camera_.adjustPos(GRAPHIC_ENGINE.camera_.getLeftVector() * camera_speed * input.dt_);
			input.actions_.erase(move_left);
		}
		auto move_right = input.actions_.find(Action::MOVE_RIGHT);
		if (move_right != input.actions_.end()) {
			GRAPHIC_ENGINE.camera_.adjustPos(GRAPHIC_ENGINE.camera_.getRightVector() * camera_speed * input.dt_);
			input.actions_.erase(move_right);
		}
		auto move_front = input.actions_.find(Action::MOVE_FRONT);
		if (move_front != input.actions_.end()) {
			GRAPHIC_ENGINE.camera_.adjustPos(GRAPHIC_ENGINE.camera_.getForwardVector() * camera_speed * input.dt_);
			input.actions_.erase(move_front);
		}
		auto move_back = input.actions_.find(Action::MOVE_BACK);
		if (move_back != input.actions_.end()) {
			GRAPHIC_ENGINE.camera_.adjustPos(GRAPHIC_ENGINE.camera_.getBackwardVector() * camera_speed * input.dt_);
			input.actions_.erase(move_back);
		}
		auto move_up = input.actions_.find(Action::MOVE_UP);
		if (move_up != input.actions_.end()) {
			GRAPHIC_ENGINE.camera_.adjustPos(0.0f, camera_speed * input.dt_, 0.0f);
			input.actions_.erase(move_up);
		}
		auto move_down = input.actions_.find(Action::MOVE_DOWN);
		if (move_down != input.actions_.end()) {
			GRAPHIC_ENGINE.camera_.adjustPos(0.0f, -camera_speed * input.dt_, 0.0f);
			input.actions_.erase(move_down);
		}

		auto look_x = input.ranges_.find(Range::LOOK_X);
		if (look_x != input.ranges_.end()) {
			GRAPHIC_ENGINE.camera_.adjustRot(0.0f, look_x->second* 0.001 * input.dt_, 0.0f);

			input.ranges_.erase(look_x);
		}

		auto look_y = input.ranges_.find(Range::LOOK_Y);
		if (look_y != input.ranges_.end()) {
			GRAPHIC_ENGINE.camera_.adjustRot(look_y->second * 0.001 * input.dt_, 0.0f, 0.0f);

			input.ranges_.erase(look_y);
		}*/
	};
	INPUT.addCallback(callback, InputCallbackPriority::HIGH);

	initializeSystems(windowContainer_.renderWindow_.getHWND(), width, height);

	return true;
}

void Engine::initializeSystems(HWND hwd, int width, int height) {
	systemManager_.addSystem<RenderSystem>(hwd, width, height);
	systemManager_.addSystem<TerrainGenerationSystem>();
	systemManager_.addSystem<TerrainMeshGenerationSystem>();
	systemManager_.addSystem<VoxelDataGenerationSystem>();
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
