#pragma once
#include "ECS/System.h"

class TestSystem : public System<TestSystem> {
public:
	TestSystem(float a = 1.5f);
	~TestSystem();
private:
	float variable_;
};

