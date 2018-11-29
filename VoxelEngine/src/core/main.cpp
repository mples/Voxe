#include <iostream>
#include "Application.h"
#include "Object.h"
#include "components/GraphicComponent.h"
#include "Camera.h"
#include "GraphicEngine.h"
#include "Input.h"


void handleInput(MappedInput& input) {
	auto found = input.actions_.find(Action::MOVE_FRONT);
	if (found != input.actions_.end()) {
		std::cout << "move front" << std::endl;
		input.eatAction(Action::MOVE_FRONT);
	}
}

int main() {
	
	Application app;

	Object block;
	block.addComponent(new GraphicComponent(new Block(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0, 20.0, 0.0), glm::vec3(1.0, 1.0, 1.0), BlockType::Grass)));
	Camera camera(glm::vec3(0.0f, 1.0f, -3.0f), glm::vec3(0.0f, 0.0f, 0.0f), 45.0f);
	GraphicEngine::getInstance().setActiveCamera(&camera);

	//input 
	InputContext* input_context = new InputContext();

	Input::getInstance().getMapper().addFrontContext(input_context);
	Input::getInstance().getMapper().addCallback(handleInput, Priority::HIGH);
	while (1) {
		app.run();

	}

	return 0;
}