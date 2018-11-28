#include "Input.h"


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	std::cout << key << ' ' << scancode << ' ' << action << ' ' << mods << std::endl;
}

Input::Input() : Singleton<Input>() {
	glfwSetKeyCallback(GraphicEngine::getInstance().getWindow(), key_callback);
}

void Input::processInput(GLFWwindow * window) {
	for (auto it : keybinds_) {
		if (glfwGetKey(window, it.first) == GLFW_PRESS) {
			it.second();
		}
	}
}