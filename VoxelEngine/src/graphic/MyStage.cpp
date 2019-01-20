#include "MyStage.h"

#include "ObjectManager.h"
#include "StageManager.h"
#include <functional>
#include "SimpleGenerator.h"
#include "PerlinNoise.h"

MyStage::MyStage() : Stage(), world_(new SimpleGenerator(new PerlinNoise(1234)) ) {
	
}


MyStage::~MyStage() {
}

void MyStage::init() {
	player_ = new Player();
	
	ObjectManager::getInstance().addObject(player_);
	
	InputContext * input_context = new InputContext();

	
	input_context->addActionMapping(Action::QUIT, RawButton(GLFW_KEY_ESCAPE));

	Input::getInstance().getMapper().pushBackContext(input_context);

	Input::getInstance().getMapper().addCallback([=](MappedInput& input) {
		auto found = std::find(input.actions_.begin(), input.actions_.end(), Action::QUIT);
		if (found != input.actions_.end()) {
			StageManager::getInstance().quitGame();
			input.eatAction(Action::QUIT);
		}
	}, InputCallbackPriority::HIGH);
	

	
}

void MyStage::update(float dt) {
	
}

void MyStage::shutdown() {
	ObjectManager::getInstance().removeObject(object_);
	ObjectManager::getInstance().removeObject(player_);

	delete player_;
	delete object_;
}
