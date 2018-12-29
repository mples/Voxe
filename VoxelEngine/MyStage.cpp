#include "MyStage.h"

#include "ObjectManager.h"
#include "StageManager.h"
#include <functional>

MyStage::MyStage() : Stage() {
	
}


MyStage::~MyStage() {
}

void MyStage::init() {
	player_ = new Player();
	/*
	object_ = new Object();
	object_->addComponent(new GraphicComponent(new Block(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0, 20.0, 0.0), glm::vec3(1.0, 1.0, 1.0), BlockType::GRASS)));
	object_->addComponent(new GraphicComponent(new Block(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0, 20.0, 0.0), glm::vec3(1.0, 1.0, 1.0), BlockType::GRASS)));
	object_->addComponent(new GraphicComponent(new Block(glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0, 20.0, 0.0), glm::vec3(1.0, 1.0, 1.0), BlockType::GRASS)));
	object_->addComponent(new GraphicComponent(new Block(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0, 20.0, 0.0), glm::vec3(1.0, 1.0, 1.0), BlockType::GRASS)));
	object_->addComponent(new GraphicComponent(new Block(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0, 20.0, 0.0), glm::vec3(1.0, 1.0, 1.0), BlockType::GRASS)));
	object_->addComponent(new GraphicComponent(new Block(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0, 20.0, 0.0), glm::vec3(1.0, 1.0, 1.0), BlockType::GRASS)));
	object_->addComponent(new GraphicComponent(new Block(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0, 20.0, 0.0), glm::vec3(1.0, 1.0, 1.0), BlockType::GRASS)));
	*/
	
	//ObjectManager::getInstance().addObject(object_);
	ObjectManager::getInstance().addObject(player_);
	
	InputContext * input_context = new InputContext();

	world_.setBlock(1, 1, 1, BlockType::GRASS);
	world_.setBlock(1, 2, 1, BlockType::GRASS);
	world_.setBlock(1, 1, 2, BlockType::GRASS);
	world_.setBlock(0, 0, 0, BlockType::GRASS);
	world_.setBlock(1, 0, 0, BlockType::GRASS);
	world_.setBlock(15, 0, 0, BlockType::GRASS);
	world_.setBlock(0, 0, 15, BlockType::GRASS);
	world_.setBlock(0, 15, 0, BlockType::GRASS);
	world_.setBlock(16, 0, 0, BlockType::GRASS);
	world_.setBlock(-1, 0, 0, BlockType::GRASS);
	world_.setBlock(-1, -1, -1, BlockType::GRASS);

	
	input_context->addActionMapping(Action::QUIT, RawButton(GLFW_KEY_ESCAPE));

	Input::getInstance().getMapper().pushBackContext(input_context);

	Input::getInstance().getMapper().addCallback([=](MappedInput& input) {
		auto found = std::find(input.actions_.begin(), input.actions_.end(), Action::QUIT);
		if (found != input.actions_.end()) {
			StageManager::getInstance().quitGame();
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
