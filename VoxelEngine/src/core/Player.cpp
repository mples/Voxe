#include "Player.h"



Player::Player() : Object() {
	CameraComponent* camera_com = new CameraComponent(this);
	camera_com->setParent(this);
	addComponent(camera_com);


	InputComponent* input_com = new InputComponent();
	addComponent(input_com);
	InputCallback callback = [=](MappedInput& input) {
		this->handleInput(input);
	};
		//std::bind(&Player::handleInput, this, _1)  ;
	auto pair = std::make_pair(callback, InputCallbackPriority::HIGH);
	input_com->addCallback(pair);
	input_com->setContext(new InputContext());
	input_com->activate();

}


Player::~Player() {
}

void Player::update(float dt) {
	Object::update(dt);
}

void Player::handleInput(MappedInput& input) {
	auto found = input.actions_.find(Action::MOVE_FRONT);
	if (found != input.actions_.end()) {
		std::cout << "move front" << std::endl;

		input.eatAction(Action::MOVE_FRONT);
	}

	auto found_back = input.actions_.find(Action::MOVE_BACK);
	if (found_back != input.actions_.end()) {
		std::cout << "move front" << std::endl;
		//setPosition(getPosition);

		input.eatAction(Action::MOVE_BACK);
	}

	auto found_left = input.actions_.find(Action::MOVE_LEFT);
	if (found_left != input.actions_.end()) {
		std::cout << "move front" << std::endl;

		input.eatAction(Action::MOVE_LEFT);
	}

	auto found_right = input.actions_.find(Action::MOVE_RIGHT);
	if (found_right != input.actions_.end()) {
		std::cout << "move front" << std::endl;

		input.eatAction(Action::MOVE_RIGHT);
	}



	auto found2 = input.ranges_.find(Range::LOOK_X);
	if (found2 != input.ranges_.end()) {
		std::cout << "Look x: " << found2->second << std::endl;
		input.eatRange(Range::LOOK_X);
	}
}

