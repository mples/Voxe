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
	Block::blockShader_ = new Shader("src/shaders/vert_shader.glsl", "src/shaders/frag_shader.glsl");
}

GraphicEngine::~GraphicEngine() {
	glfwTerminate();
}

void GraphicEngine::draw() {
	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto comp : components_) {
		comp->draw();
	}

	glfwSwapBuffers(window_);
	glfwPollEvents();

	if (glfwWindowShouldClose(window_)) {
		exit(0);
	}
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

void GraphicEngine::registerComponent(GraphicComponent * g_comp) {
	auto found = std::find(components_.begin(), components_.end(), g_comp);
	if (found == components_.end()) {
		components_.push_back(g_comp);
	}
}

void GraphicEngine::unregisterComponent(GraphicComponent * g_comp) {
	auto found = std::find(components_.begin(), components_.end(), g_comp);
	if (found != components_.end()) {
		components_.erase(found);
	}
}

void GraphicEngine::setActiveCamera(Camera * camera) {
	activeCamera_ = camera;
}

Camera * GraphicEngine::getActiveCamera() {
	return activeCamera_;
}
