#include "GraphicEngine.h"
#include <iostream>

#include <glad/glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>

#include "Block.h"
#include "BlockType.h"
#include "Shader.h"

void framebuffer_size_callback(GLFWwindow* window, int height, int width) {
	glViewport(0, 0, height, width);
}

void GraphicEngine::draw() {
	glm::mat4 trans;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(windowHeight_, windowWidth_, windowTitle_.c_str(), nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to cearte window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to inicialize glad" << std::endl;
		return;
	}

	glViewport(0, 0, windowHeight_, windowWidth_);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	Shader shader("src/shaders/vert_shader.glsl", "src/shaders/frag_shader.glsl");
	shader.activate();
	shader.setInt("texture1", 0);
	
	Block block(glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0), BlockType::Grass);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		block.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return;
}
