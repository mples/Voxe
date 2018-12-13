#pragma once
#include "StageManager.h"
#include "ObjectManager.h"
#include "Player.h"

class Application {
public:
	Application();
	~Application();

	void run();
private:
	bool shouldClose_ = false;
};

