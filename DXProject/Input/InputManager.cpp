#include "InputManager.h"
#include <iostream>
//
//static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
//	//std::cout << key << " " << scancode << " " << action << " " << mods << std::endl;
//	RawButton button (key, scancode, action, mods);
//	Input::getInstance().getMapper().setRawButtons(button);
//}
//
//static bool firstMouseMove = true;
//static void mouse_position_callback(GLFWwindow* window, double xpos, double ypos) {
//	static double prev_xpos;
//	static double prev_ypos;
//	if (firstMouseMove) {
//		prev_xpos = xpos;
//		prev_ypos = ypos;
//		firstMouseMove = false;
//	}
//	//std::cout << "Raw: " << xpos << " " << ypos << std::endl;
//
//	RawAxis x_axis (AxisType::MOUSE_X, xpos - prev_xpos);
//	RawAxis y_axis(AxisType::MOUSE_Y, prev_ypos - ypos);
//	//std::cout << "Offset: " <<xpos - prev_xpos << " " << prev_ypos - ypos << std::endl;
//	prev_xpos = xpos;
	//prev_ypos = ypos;
//
//
//	Input::getInstance().getMapper().setRawAxis(x_axis);
//	Input::getInstance().getMapper().setRawAxis(y_axis);
//}

InputManager::InputManager() : Singleton<InputManager>() {
}

void InputManager::processInput() {
	dispatchEvents();
	mapper_.dispatch();
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
			if (MOUSE.isRightButtonDown()) { //debug
				RawAxis raw_mouse_x(AxisType::RAW_INPUT_MOUSE_X, mouse_event.getPosX());
				mapper_.axisMoved(raw_mouse_x);
				RawAxis raw_mouse_y(AxisType::RAW_INPUT_MOUSE_Y, mouse_event.getPosY());
				mapper_.axisMoved(raw_mouse_y);
				break;
			}
		}
		}
		if (mouse_event.getType() == MouseEvent::RAW_MOVE) {
			//gfxEngine_.camera_.adjustRot(mouse_event.getPosY() * 0.01f, mouse_event.getPosX() * 0.01f, 0.0f);
		}


	}
}
