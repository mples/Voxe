#include "Engine.h"



Engine::Engine() {
}


Engine::~Engine() {
}

bool Engine::init(HINSTANCE hInstance, std::wstring window_title, std::wstring window_class, int width, int height) {
	if (!renderWindow_.initialize(this, hInstance, window_title, window_class, width, height)) {
		return false;
	}

	if (!gfxEngine_.initialize(renderWindow_.getHWND(), width, height)) {
		return false;
	}

	timer_.start();

	return true;
}

bool Engine::processMessages() {
	return renderWindow_.procesMessages();
}

void Engine::update() {
	double dt = timer_.getTimePassedMilisec();
	timer_.restart();

	while (!keyboardManager_.charBufferIsEmpty()) {
		unsigned char ch = keyboardManager_.readChar();
	}
	while (!keyboardManager_.keyBufferIsEmpty()) {
		KeyboardEvent key_event = keyboardManager_.readKey();
		unsigned char keycode = key_event.getKeyCode();
	}
	while (!mouseManager_.isEventBufferEmpty()) {
		MouseEvent mouse_event = mouseManager_.readEvent();
		if (mouseManager_.isRightButtonDown()) {
			if (mouse_event.getType() == MouseEvent::RAW_MOVE) {
				gfxEngine_.camera_.adjustRot(mouse_event.getPosY() * 0.01f, mouse_event.getPosX() * 0.01f, 0.0f);
			}
		}
	}

	const float camera_speed = 0.01f;

	if (keyboardManager_.keyIsPressed('W')) {
		gfxEngine_.camera_.adjustPos(gfxEngine_.camera_.getForwardVector() * camera_speed * dt);
	}
	if (keyboardManager_.keyIsPressed('S')) {
		gfxEngine_.camera_.adjustPos(gfxEngine_.camera_.getBackwardVector() * camera_speed * dt);
	}
	if (keyboardManager_.keyIsPressed('D')) {
		gfxEngine_.camera_.adjustPos(gfxEngine_.camera_.getRightVector() * camera_speed * dt);
	}
	if (keyboardManager_.keyIsPressed('A')) {
		gfxEngine_.camera_.adjustPos(gfxEngine_.camera_.getLeftVector() * camera_speed * dt);
	}
	if (keyboardManager_.keyIsPressed(VK_SPACE)) {
		gfxEngine_.camera_.adjustPos(0.0f, camera_speed * dt, 0.0f);
	}
	if (keyboardManager_.keyIsPressed('Z')) {
		gfxEngine_.camera_.adjustPos(0.0f, -camera_speed * dt, 0.0f);
	}

	if (keyboardManager_.keyIsPressed('C')) {
		gfxEngine_.light_.setPos(gfxEngine_.camera_.getPos());
		gfxEngine_.light_.setRot(gfxEngine_.camera_.getRot());
	}

	gfxEngine_.draw();
}
