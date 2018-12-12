#include "Player.h"

#include<glfw3.h>

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

	InputContext * input_context = new InputContext();
	/*
	input_context->addActionMapping(Action::MOVE_FRONT, RawButton(GLFW_KEY_W));
	input_context->addActionMapping(Action::MOVE_BACK, RawButton(GLFW_KEY_S));
	input_context->addActionMapping(Action::MOVE_LEFT, RawButton(GLFW_KEY_A));
	input_context->addActionMapping(Action::MOVE_RIGHT, RawButton(GLFW_KEY_D));*/

	input_context->addStateMapping(State::MOVING_FRONT, RawButton(GLFW_KEY_W));
	input_context->addStateMapping(State::MOVING_BACK, RawButton(GLFW_KEY_S));
	input_context->addStateMapping(State::MOVING_LEFT, RawButton(GLFW_KEY_A));
	input_context->addStateMapping(State::MOVING_RIGHT, RawButton(GLFW_KEY_D));
	
	input_com->setContext(input_context);
	input_com->activate();

}


Player::~Player() {
}

void Player::update(float dt) {
	Object::update(dt);
}

void Player::handleInput(MappedInput& input) {
	/*
	auto found = input.actions_.find(Action::MOVE_FRONT);
	if (found != input.actions_.end()) {
		std::cout << "move front" << std::endl;
		setPosition(getPosition() + glm::vec3(0.01f, 0.0f, -0.1f));

		input.eatAction(Action::MOVE_FRONT);
	}

	auto found_back = input.actions_.find(Action::MOVE_BACK);
	if (found_back != input.actions_.end()) {
		std::cout << "move back" << std::endl;
		setPosition(getPosition() + glm::vec3(0.0f, 0.0f, 0.1f));

		input.eatAction(Action::MOVE_BACK);
	}

	auto found_left = input.actions_.find(Action::MOVE_LEFT);
	if (found_left != input.actions_.end()) {
		std::cout << "move left" << std::endl;
		setPosition(getPosition() + glm::vec3(-0.1f, 0.0f, 0.0f));

		input.eatAction(Action::MOVE_LEFT);
	}

	auto found_right = input.actions_.find(Action::MOVE_RIGHT);
	if (found_right != input.actions_.end()) {
		std::cout << "move right" << std::endl;
		setPosition(getPosition() + glm::vec3(0.1f, 0.0f, 0.0f));

		input.eatAction(Action::MOVE_RIGHT);
	}*/

	auto found_right = input.states_.find(State::MOVING_RIGHT);
	if (found_right != input.states_.end()) {
		std::cout << "move right" << std::endl;
		setPosition(getPosition() + glm::vec3(0.1f, 0.0f, 0.0f));

		input.eatState(State::MOVING_RIGHT);
	}
	auto found_left = input.states_.find(State::MOVING_LEFT);
	if (found_left != input.states_.end()) {
		std::cout << "move left" << std::endl;
		setPosition(getPosition() + glm::vec3(-0.1f, 0.0f, 0.0f));

		input.eatState(State::MOVING_LEFT);
	}
	auto found_front = input.states_.find(State::MOVING_FRONT);
	if (found_front != input.states_.end()) {
		std::cout << "move front" << std::endl;
		setPosition(getPosition() + glm::vec3(0.0f, 0.0f, -0.1f));

		input.eatState(State::MOVING_FRONT);
	}
	auto found_back = input.states_.find(State::MOVING_BACK);
	if (found_back != input.states_.end()) {
		std::cout << "move back" << std::endl;
		setPosition(getPosition() + glm::vec3(0.0f, 0.0f, -0.1f));

		input.eatState(State::MOVING_BACK);
	}


	auto found2 = input.ranges_.find(Range::LOOK_X);
	if (found2 != input.ranges_.end()) {
		std::cout << "Look x: " << found2->second << std::endl;
		input.eatRange(Range::LOOK_X);
	}
}

