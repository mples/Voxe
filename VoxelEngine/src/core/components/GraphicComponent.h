#pragma once

#include "Component.h"
#include "Block.h"

class GraphicComponent : public Component {
public:
	GraphicComponent();
	~GraphicComponent();

	virtual void update(float dt);
	void draw();
private:
	
};