#include<glad\glad.h>
#include<glfw3.h>

#include "GraphicEngine.h"
#include "Input.h"

int main() {
	
	GraphicEngine::createInstance();

	Input::createInstance();

	GraphicEngine::getInstance().draw();

	GraphicEngine::removeInstance();


	return 0;
}