#include<glad\glad.h>
#include<glfw3.h>

#include "GraphicEngine.h"

int main() {
	
	GraphicEngine::createInstance();

	GraphicEngine::getInstance().draw();

	GraphicEngine::removeInstance();


	return 0;
}