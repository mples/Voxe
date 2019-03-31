#include "GraphicComponent.h"
#include "GraphicEngine.h"

GraphicComponent::GraphicComponent(Block* block) : Component(ComponentType::GraphicComponent) {
	GraphicEngine::getInstance().registerComponent(this);
	blocks_.push_back(block);
}

GraphicComponent::GraphicComponent(std::vector<Block*>& blocks) : Component(ComponentType::GraphicComponent) {
	GraphicEngine::getInstance().registerComponent(this);
	blocks_ = std::vector<Block*>(blocks);
}

GraphicComponent::~GraphicComponent() {
	GraphicEngine::getInstance().unregisterComponent(this);
}

void GraphicComponent::update(float dt) {
}

void GraphicComponent::draw() {
	for (Block* b : blocks_) {
		b->draw();
	}
}
