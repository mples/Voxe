#include "Engine.h"



Engine::Engine() {
}


Engine::~Engine() {
	MouseManager::removeInstance();
	KeyboardManager::removeInstance();
	InputManager::removeInstance();
	StageManager::removeInstance();
}

bool Engine::init(HINSTANCE hInstance, std::wstring window_title, std::wstring window_class, int width, int height) {
	if (!renderWindow_.initialize(this, hInstance, window_title, window_class, width, height)) {
		return false;
	}

	if (!gfxEngine_.initialize(renderWindow_.getHWND(), width, height)) {
		return false;
	}

	timer_.start();

	MouseManager::createInstance();
	KeyboardManager::createInstance();
	InputManager::createInstance();
	StageManager::createInstance();


	engineContext_.addActionMapping(Action::MOVE_FRONT, KeyboardEvent(KeyboardEvent::Type::PRESS, 'W'));
	engineContext_.addActionMapping(Action::MOVE_BACK, KeyboardEvent(KeyboardEvent::Type::PRESS, 'S'));
	engineContext_.addActionMapping(Action::MOVE_RIGHT, KeyboardEvent(KeyboardEvent::Type::PRESS, 'D'));
	engineContext_.addActionMapping(Action::MOVE_LEFT, KeyboardEvent(KeyboardEvent::Type::PRESS, 'A'));
	engineContext_.addActionMapping(Action::MOVE_DOWN, KeyboardEvent(KeyboardEvent::Type::PRESS, 'Z'));
	engineContext_.addActionMapping(Action::MOVE_UP, KeyboardEvent(KeyboardEvent::Type::PRESS, VK_SPACE));

	engineContext_.addRangeMapping(Range::LOOK_X, RawAxis(AxisType::RAW_INPUT_MOUSE_X));
	engineContext_.addRangeMapping(Range::LOOK_Y, RawAxis(AxisType::RAW_INPUT_MOUSE_Y));

	static float camera_speed = 0.5f;

	INPUT.addFrontContext(&engineContext_);
	InputCallback callback = [=](MappedInput& input) {
		auto move_left = input.actions_.find(Action::MOVE_LEFT);
		if (move_left != input.actions_.end()) {
			gfxEngine_.camera_.adjustPos(gfxEngine_.camera_.getLeftVector() * camera_speed);
			input.actions_.erase(move_left);
		}
		auto move_right = input.actions_.find(Action::MOVE_RIGHT);
		if (move_right != input.actions_.end()) {
			gfxEngine_.camera_.adjustPos(gfxEngine_.camera_.getRightVector() * camera_speed);
			input.actions_.erase(move_right);
		}
		auto move_front = input.actions_.find(Action::MOVE_FRONT);
		if (move_front != input.actions_.end()) {
			gfxEngine_.camera_.adjustPos(gfxEngine_.camera_.getForwardVector() * camera_speed);
			input.actions_.erase(move_front);
		}
		auto move_back = input.actions_.find(Action::MOVE_BACK);
		if (move_back != input.actions_.end()) {
			gfxEngine_.camera_.adjustPos(gfxEngine_.camera_.getBackwardVector() * camera_speed);
			input.actions_.erase(move_back);
		}
		auto move_up = input.actions_.find(Action::MOVE_UP);
		if (move_up != input.actions_.end()) {
			gfxEngine_.camera_.adjustPos(0.0f, camera_speed, 0.0f);
			input.actions_.erase(move_up);
		}
		auto move_down = input.actions_.find(Action::MOVE_DOWN);
		if (move_down != input.actions_.end()) {
			gfxEngine_.camera_.adjustPos(0.0f, -camera_speed, 0.0f);
			input.actions_.erase(move_down);
		}

		auto look_x = input.ranges_.find(Range::LOOK_X);
		if (look_x != input.ranges_.end()) {
			gfxEngine_.camera_.adjustRot(0.0f, look_x->second * 0.01f, 0.0f);

			input.ranges_.erase(look_x);
		}

		auto look_y = input.ranges_.find(Range::LOOK_Y);
		if (look_y != input.ranges_.end()) {
			gfxEngine_.camera_.adjustRot(look_y->second * 0.01f, 0.0f, 0.0f);

			input.ranges_.erase(look_y);
		}
	};
	INPUT.addCallback(callback, InputCallbackPriority::HIGH);

	return true;
}

bool Engine::processMessages() {
	return renderWindow_.procesMessages();
}

void Engine::update() {
	double dt = timer_.getTimePassedMilisec();
	timer_.restart();
	const float camera_speed = 0.01f;

	INPUT.processInput();
	STAGE_MANAGER.update(dt);

	gfxEngine_.draw();
}
