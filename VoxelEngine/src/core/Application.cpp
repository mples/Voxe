#include "Application.h"
#include "GraphicEngine.h"
#include "Input.h"
#include "Object.h"
#include "components/GraphicComponent.h"
#include "Player.h"

Application::Application() {
	GraphicEngine::createInstance();
	Input::createInstance();
}


Application::~Application() {
}

void Application::run() {
	Player player;
	player.setPosition(glm::vec3(0.0f, 0.0f, -3.0f));
	while ( !shouldClose_) {
		player.update(10.0f);
		GraphicEngine::getInstance().draw();
		Input::getInstance().processInput();
	}
}
