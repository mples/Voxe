#pragma once
#include <glad/glad.h>
#include <glfw3.h>

#include <unordered_map>
#include <functional>
#include <iostream>

#include "GraphicEngine.h"
#include "Singleton.h"

class Input : public Singleton<Input> {
public:
	Input();
	void processInput(GLFWwindow *window);
private:
	std::unordered_map<int, std::function<void()> > keybinds_;
};

