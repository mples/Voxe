#pragma once

#include "Component.h"
#include "Block.h"

class GraphicComponent : public Component {
public:
	GraphicComponent(Block* block);
	GraphicComponent(std::vector<Block*>& blocks);
	~GraphicComponent();

	virtual void update(float dt);
	void draw();
private:
	std::vector<Block*> blocks_;
};