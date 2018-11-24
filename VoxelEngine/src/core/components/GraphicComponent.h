#pragma once

#include "Component.h"

class GraphicComponent : public Component {
public:
	GraphicComponent();
	~GraphicComponent();

	virtual void update(float dt);
	void draw();
private:

};