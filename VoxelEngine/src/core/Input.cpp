#include "Input.h"
#include <iostream>

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	std::cout << key << " " << scancode << " " << action << " " << mods << std::endl;
	RawButton button (key, scancode, action, mods);
	Input::getInstance().getMapper().setRawButtons(button);
}

Input::Input() : Singleton<Input>() {
	glfwSetKeyCallback(GraphicEngine::getInstance().getWindow(), key_callback);
}

void Input::processInput() {
	mapper_.dispatch();
}

InputMapper & Input::getMapper() {
	return mapper_;
}
