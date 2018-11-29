#include "Application.h"
#include "GraphicEngine.h"
#include "Input.h"
#include "Object.h"
#include "components/GraphicComponent.h"

Application::Application() {
	GraphicEngine::createInstance();
	Input::createInstance();
}


Application::~Application() {
}

void Application::run() {
	while ( !shouldClose_) {
		GraphicEngine::getInstance().draw();
		Input::getInstance().processInput();
	}
}
