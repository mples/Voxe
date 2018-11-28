#include "GraphicEngine.h"


void framebuffer_size_callback(GLFWwindow* window, int height, int width) {
	glViewport(0, 0, height, width);
}

GraphicEngine::GraphicEngine() : Singleton<GraphicEngine>(){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window_ = glfwCreateWindow(windowHeight_, windowWidth_, windowTitle_.c_str(), nullptr, nullptr);
	if (window_ == nullptr) {
		std::cout << "Failed to cearte window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window_);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to inicialize glad" << std::endl;
		return;
	}

	glViewport(0, 0, windowHeight_, windowWidth_);
	glEnable(GL_DEPTH_TEST);
	glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);
}

void GraphicEngine::draw() {


	Shader shader("src/shaders/vert_shader.glsl", "src/shaders/frag_shader.glsl");
	shader.activate();
	shader.setInt("texture1", 0);
	
	Block block(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0, 20.0, 0.0), glm::vec3(1.0, 1.0, 1.0), BlockType::Grass);

	Camera camera(glm::vec3(0.0f, 1.0f, -3.0f), glm::vec3(0.0f, 0.0f, 0.0f), 45.0f);

	while (!glfwWindowShouldClose(window_)) {
		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.setMat4("modelMatrix", block.getModelMatrix());
		shader.setMat4("viewMatrix", camera.getViewMatrix());
		shader.setMat4("projMatrix", camera.getProjMatrix());
		block.draw();

		glfwSwapBuffers(window_);
		glfwPollEvents();
	}

	glfwTerminate();
	return;
}

int GraphicEngine::getWindowHeight() {
	return windowHeight_;
}

int GraphicEngine::getWindowWidth() {
	return windowWidth_;
}

GLFWwindow* GraphicEngine::getWindow() {
	return window_;
}
