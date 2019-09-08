#include "InputManager.h"
#include <iostream>

InputManager::InputManager() : Singleton<InputManager>() {
}

void InputManager::processInput(float dt) {
	dispatchEvents();
	mapper_.dispatch(dt);
}

void InputManager::addCallback(InputCallback callback, InputCallbackPriority priority) {
	mapper_.addCallback(callback, priority);
}

void InputManager::removeCallback(InputCallback callback) {
	mapper_.removeCallback(callback);
}

void InputManager::pushBackContext(InputContext * input_context) {
	mapper_.pushBackContext(input_context);
}

void InputManager::removeContext(InputContext * input_context) {
	mapper_.removeContext(input_context);
}

void InputManager::addFrontContext(InputContext * input_context) {
	mapper_.addFrontContext(input_context);
}

void InputManager::moveFrontContext(InputContext * input_context) {
	mapper_.moveFrontContext(input_context);
}

void InputManager::moveBackContext(InputContext * input_context) {
	mapper_.moveBackContext(input_context);
}

float InputManager::roundDiff(float value) {
	return fabsf(value - roundf(value));
}

void InputManager::dispatchEvents() {
	while (!KEYBOARD.charBufferIsEmpty()) {
		unsigned char ch = KEYBOARD.readChar();
	}
	while (!KEYBOARD.keyBufferIsEmpty()) {
		KeyboardEvent key_event = KEYBOARD.readKey();
		mapper_.buttonPressed(key_event);
	}
	while (!MOUSE.isEventBufferEmpty()) {
		MouseEvent mouse_event = MOUSE.readEvent();
		switch (mouse_event.getType()) {
			case MouseEvent::Type::RAW_MOVE: {
				RawAxis raw_mouse_x(AxisType::RAW_INPUT_MOUSE_X, mouse_event.getPosX());
				mapper_.axisMoved(raw_mouse_x);
				RawAxis raw_mouse_y(AxisType::RAW_INPUT_MOUSE_Y, mouse_event.getPosY());
				mapper_.axisMoved(raw_mouse_y);
				break;
			}
		}
	}
}
