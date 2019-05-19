#pragma once
#include "ECS/Component.h"

class TestComponent : public Component<TestComponent> {
public:
	TestComponent(float time = 0.0f);
	~TestComponent();
private:
	float time_ = 0.01f;
};

