#pragma once
#include <glad/glad.h>
#include <glfw3.h>

#include <unordered_map>
#include <functional>
#include <iostream>

#include "GraphicEngine.h"
#include "Singleton.h"
#include "InputMapper.h"

class Input : public Singleton<Input> {
public:
	Input();
	void processInput();
	InputMapper& getMapper();
private:
	InputMapper mapper_;
};

