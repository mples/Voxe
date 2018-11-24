#pragma once
#include <string>
#include "Singleton.h"

class GraphicEngine : public Singleton<GraphicEngine>
{
public:
	void draw();

private:
	int windowHeight_ = 800;
	int windowWidth_ = 600;
	std::string windowTitle_ = "Voxel Engine";


};

