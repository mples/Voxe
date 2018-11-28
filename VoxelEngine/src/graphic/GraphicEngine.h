#pragma once
#include <glad/glad.h>
#include <glfw3.h>

#include <glm/glm.hpp>

#include <string>
#include "Singleton.h"
#include <iostream>

#include "Block.h"
#include "BlockType.h"
#include "Shader.h"
#include "Camera.h"

class GraphicEngine : public Singleton<GraphicEngine>
{
public:
	GraphicEngine();
	void draw();

	int getWindowHeight();
	int getWindowWidth();
	GLFWwindow * getWindow();
private:
	int windowHeight_ = 800;
	int windowWidth_ = 600;
	std::string windowTitle_ = "Voxel Engine";
	GLFWwindow* window_;

};

