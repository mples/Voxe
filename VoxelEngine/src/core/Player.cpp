#include "Player.h"

#include<glfw3.h>

Player::Player() : frontVec_(glm::vec3(0.0f, 0.0f, -1.0f)), rightVec_(glm::vec3(1.0f, 0.0f, 0.0f)), Object() {
	rotation_.y = 90.0f;
	CameraComponent* camera_com = new CameraComponent(this);
	camera_com->setParent(this);
	addComponent(camera_com);


	InputComponent* input_com = new InputComponent();
	addComponent(input_com);
	InputCallback callback = [=](MappedInput& input) {
		this->handleInput(input);
	};

	auto pair = std::make_pair(callback, InputCallbackPriority::HIGH);
	input_com->addCallback(pair);

	InputContext * input_context = new InputContext();

	input_context->addStateMapping(State::MOVING_FRONT, RawButton(GLFW_KEY_W));
	input_context->addStateMapping(State::MOVING_BACK, RawButton(GLFW_KEY_S));
	input_context->addStateMapping(State::MOVING_LEFT, RawButton(GLFW_KEY_A));
	input_context->addStateMapping(State::MOVING_RIGHT, RawButton(GLFW_KEY_D));
	input_context->addActionMapping(Action::LMOUSE_CLICK, RawButton(GLFW_KEY_Q));

	input_context->addRangeMapping(Range::LOOK_X, RawAxis(AxisType::MOUSE_X));
	input_context->addRangeMapping(Range::LOOK_Y, RawAxis(AxisType::MOUSE_Y));

	input_context->addRangeConverter(Range::LOOK_X, RangeConverter(-10, 10, 0, 3, 1));
	input_context->addRangeConverter(Range::LOOK_Y, RangeConverter(-10, 10, 0, 3, 1));

	input_com->setContext(input_context);
	input_com->activate();

}


Player::~Player() {
}

void Player::update(float dt) {
	Object::update(dt);
	position_ += velocity_;
}

void Player::handleInput(MappedInput& input) {

	float speed = 0.5f;
	glm::vec3 new_velocity(0.0f, 0.0f,0.0f);
	auto found_right = input.states_.find(State::MOVING_RIGHT);
	if (found_right != input.states_.end()) {
		//std::cout << "move right" << std::endl;
		new_velocity += rightVec_ * speed;

		input.eatState(State::MOVING_RIGHT);
	}
	auto found_left = input.states_.find(State::MOVING_LEFT);
	if (found_left != input.states_.end()) {
		//std::cout << "move left" << std::endl;
		new_velocity -= rightVec_ * speed;

		input.eatState(State::MOVING_LEFT);
	}
	auto found_front = input.states_.find(State::MOVING_FRONT);
	if (found_front != input.states_.end()) {
		//std::cout << "move front" << std::endl;
		new_velocity += frontVec_ * speed;

		input.eatState(State::MOVING_FRONT);
	}
	auto found_back = input.states_.find(State::MOVING_BACK);
	if (found_back != input.states_.end()) {
		//std::cout << "move back" << std::endl;
		new_velocity -= frontVec_ * speed;

		input.eatState(State::MOVING_BACK);
	}


	auto foundx = input.ranges_.find(Range::LOOK_X);
	if (foundx != input.ranges_.end()) {
		//std::cout << "Look x: " << foundx->second << std::endl;
		rotation_.y += foundx->second;
		updateVectors();

		input.eatRange(Range::LOOK_X);
	}

	auto foundy = input.ranges_.find(Range::LOOK_Y);
	if (foundy != input.ranges_.end()) {
		//std::cout << "Look y: " << foundy->second << std::endl;
		rotation_.x += foundy->second;
		updateVectors();

		input.eatRange(Range::LOOK_Y);
	}

	velocity_ = new_velocity;

	auto mouse_clicked = input.actions_.find(Action::LMOUSE_CLICK);
	if (mouse_clicked != input.actions_.end()) {
		glm::vec3 selected = GraphicEngine::getInstance().unprojectMiddlePixel();
		int x = floorf(selected.x);
		int y = floorf(selected.y);
		int z = floorf(selected.z);
		//std::cout << "x: " << x << " y: " << y << " z: " << z << std::endl;
		GraphicEngine::getInstance().getActiveWorld()->setBlock(x, y, z, BlockType::AIR);
		
		input.eatAction(Action::LMOUSE_CLICK);
	}
}

void Player::setPosition(glm::vec3& pos) {
	position_ = pos;
}

void Player::setRotation(glm::vec3& rot) {
	rotation_ = rot;
	updateVectors();
}

void Player::updateVectors() {

	glm::vec3 front;
	front.x = cos(glm::radians(rotation_.y)) * cos(glm::radians(rotation_.x));
	front.y = sin(glm::radians(rotation_.x));
	front.z = sin(glm::radians(rotation_.y)) * cos(glm::radians(rotation_.x));
	frontVec_ = glm::normalize(front);
	
	rightVec_ = glm::normalize(glm::cross(frontVec_, glm::vec3(0.0f, 1.0f, 0.0f)));
}
