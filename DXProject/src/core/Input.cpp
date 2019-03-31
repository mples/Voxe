#include "Input.h"
#include <iostream>

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//std::cout << key << " " << scancode << " " << action << " " << mods << std::endl;
	RawButton button (key, scancode, action, mods);
	Input::getInstance().getMapper().setRawButtons(button);
}

static bool firstMouseMove = true;
static void mouse_position_callback(GLFWwindow* window, double xpos, double ypos) {
	static double prev_xpos;
	static double prev_ypos;
	if (firstMouseMove) {
		prev_xpos = xpos;
		prev_ypos = ypos;
		firstMouseMove = false;
	}
	//std::cout << "Raw: " << xpos << " " << ypos << std::endl;

	RawAxis x_axis (AxisType::MOUSE_X, xpos - prev_xpos);
	RawAxis y_axis(AxisType::MOUSE_Y, prev_ypos - ypos);
	//std::cout << "Offset: " <<xpos - prev_xpos << " " << prev_ypos - ypos << std::endl;
	prev_xpos = xpos;
	prev_ypos = ypos;


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

glm::vec3 Input::unprojectMiddlePixel() {
	float depth;
	int window_width = GraphicEngine::getInstance().getWindowWidth();
	int window_height = GraphicEngine::getInstance().getWindowHeight();

	glReadPixels(window_width / 2, window_height / 2, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

	glm::vec4 viewport = glm::vec4(0, 0, window_width, window_height);
	glm::vec3 win_coord = glm::vec3(window_width / 2, window_height / 2, depth);
	Camera * active_camera = GraphicEngine::getInstance().getActiveCamera();
	glm::vec3 selected_coord = glm::unProject(win_coord, active_camera->getViewMatrix(), active_camera->getProjMatrix(), viewport);


	return selected_coord;
}

glm::ivec3 Input::unprojectMiddleVoxel() {
	glm::vec3 middle_coord = unprojectMiddlePixel();
	int x = floorf(middle_coord.x);
	int y = floorf(middle_coord.y);
	int z = floorf(middle_coord.z);

	return glm::ivec3(x, y, z);
}

glm::ivec3 Input::unprojectNextToMiddleVoxel() {
	glm::vec3 camera_pos = GraphicEngine::getInstance().getActiveCamera()->getPosition();
	glm::vec3 middle_coord = unprojectMiddlePixel();
	//getting middle voxel
	int x = floorf(middle_coord.x);
	int y = floorf(middle_coord.y);
	int z = floorf(middle_coord.z);

	float x_diff = roundDiff(middle_coord.x);
	float y_diff = roundDiff(middle_coord.y);
	float z_diff = roundDiff(middle_coord.z);

	float minimum = std::min({x_diff, y_diff, z_diff });

	if (minimum == x_diff) {
		if (camera_pos.x >= middle_coord.x) {
			return glm::ivec3(x + 1, y, z);
		}
		else {
			return glm::ivec3(x - 1, y, z);
		}
	}
	else if (minimum == y_diff) {
		if (camera_pos.y >= middle_coord.y) {
			return glm::ivec3(x, y + 1, z);
		}
		else {
			return glm::ivec3(x, y - 1, z);
		}
	}
	else {
		if (camera_pos.z >= middle_coord.z) {
			return glm::ivec3(x, y, z + 1);
		}
		else {
			return glm::ivec3(x, y, z - 1);
		}
	}
}

float Input::roundDiff(float value) {
	return fabsf(value - roundf(value));
}
