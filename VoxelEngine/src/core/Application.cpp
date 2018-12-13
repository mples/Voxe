#include "Application.h"

Application::Application() {
	GraphicEngine::createInstance();
	Input::createInstance();
	StageManager::createInstance();
	ObjectManager::createInstance();
}


Application::~Application() {
}

void Application::run() {
	while ( !shouldClose_) {
		StageManager::getInstance().update(10.0f);

		if (StageManager::getInstance().isQuitting()) {
			exit(0);
		}
		//player.update(10.0f);
		//GraphicEngine::getInstance().draw();
		//Input::getInstance().processInput();

	}
}
