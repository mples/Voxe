#include <iostream>
#include "Application.h"
#include "Object.h"
#include "components/GraphicComponent.h"
#include "Camera.h"
#include "GraphicEngine.h"
#include "Input.h"
#include "Player.h"
#include "MyStage.h"

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
	MyStage* my_stage = new MyStage();
	StageManager::getInstance().setStartingStage(my_stage);
	app.run();

	return 0;
}