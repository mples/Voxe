#include <iostream>
#include "Application.h"
#include "Object.h"
#include "components/GraphicComponent.h"
#include "Camera.h"
#include "GraphicEngine.h"
#include "Input.h"
#include "Player.h"

void handleInput(MappedInput& input) {
	static long counter = 0;
	static long counter2 = 0;
	auto found = input.actions_.find(Action::MOVE_FRONT);
	if (found != input.actions_.end()) {
		std::cout << "move front" << std::endl;
		input.eatAction(Action::MOVE_FRONT);
	}

	auto found2 = input.ranges_.find(Range::LOOK_X);
	if (found2 != input.ranges_.end()) {
		std::cout << "Look x: " << found2->second << std::endl;
		input.eatRange(Range::LOOK_X);
		++counter2;
	}
	++counter;
	std::cout << "Counter: " << counter << "Counter2: "<< counter2 << std::endl;

}

int main() {
	
	Application app;

	//Player player;
	//player.setPosition(glm::vec3(0.0f, 1.0f, -3.0f));

	Object block;
	block.addComponent(new GraphicComponent(new Block(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0, 20.0, 0.0), glm::vec3(1.0, 1.0, 1.0), BlockType::Grass)));
	//Camera camera(glm::vec3(0.0f, 1.0f, -3.0f), glm::vec3(0.0f, 0.0f, 0.0f), 45.0f);
	///GraphicEngine::getInstance().setActiveCamera(&camera);

	//input 
	//InputContext* input_context = new InputContext();

	//Input::getInstance().getMapper().addFrontContext(input_context);
//	Input::getInstance().getMapper().addCallback(handleInput, InputCallbackPriority::HIGH);



	while (1) {
		//player.update(10.f);
		app.run();
	}

	return 0;
}