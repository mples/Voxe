#pragma once
#include <glad/glad.h>
#include <glfw3.h>
#include <glm/glm.hpp>

#include <string>
#include <iostream>
#include <vector>

#include "Singleton.h"
#include "Block.h"
#include "BlockType.h"
#include "Camera.h"
#include "components/GraphicComponent.h"
#include "../../Chunk.h"
#include "../../ChunkRenderer.h"
#include "../../World.h"

class GraphicEngine : public Singleton<GraphicEngine>
{
public:
	GraphicEngine();
	~GraphicEngine();
	void draw();

	int getWindowHeight();
	int getWindowWidth();
	GLFWwindow * getWindow();

	void registerComponent(GraphicComponent* g_comp);
	void unregisterComponent(GraphicComponent* g_comp);

	World* getActiveWorld();
	void setWorld(World * world);

	void setActiveCamera(Camera* camera);
	Camera* getActiveCamera();
private:
	int windowHeight_ = 1200;
	int windowWidth_ = 1200;
	std::string windowTitle_ = "Voxel Engine";
	GLFWwindow* window_;
	std::vector<GraphicComponent*> components_;
	Camera* activeCamera_;
	ChunkRenderer* chunkRenderer_;
	World* world_;
};

