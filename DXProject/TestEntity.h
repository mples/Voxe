#pragma once
#include "ECS/Entity.h"

class TestEntity :public Entity<TestEntity> {
public:
	TestEntity();
	TestEntity(int x);
	~TestEntity();
private:
	int testVariable_ = 2;

};

