#include "Input.h"
#include <iostream>

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	std::cout << key << " " << scancode << " " << action << " " << mods << std::endl;
	RawButton button (key, scancode, action, mods);
	Input::getInstance().getMapper().setRawButtons(button);
}

static void mouse_position_callback(GLFWwindow* window, double xpos, double ypos) {
	static double prev_xpos = xpos;
	static double prev_ypos = ypos;

	std::cout << xpos << " " << xpos << std::endl;

	RawAxis x_axis (AxisType::MOUSE_X, xpos - prev_xpos);
	RawAxis y_axis(AxisType::MOUSE_Y, prev_ypos - ypos);
	Input::getInstance().getMapper().setRawAxis(x_axis);
	Input::getInstance().getMapper().setRawAxis(y_axis);
}

Input::Input() : Singleton<Input>() {
	glfwSetKeyCallback(GraphicEngine::getInstance().getWindow(), key_callback);
	glfwSetCursorPosCallback(GraphicEngine::getInstance().getWindow(), mouse_position_callback);
}

void Input::processInput() {
	mapper_.dispatch();
}

InputMapper & Input::getMapper() {
	return mapper_;
}
