#include "Application.h"

Application::Application() {
	BlockManager::createInstance();
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

	}
}
