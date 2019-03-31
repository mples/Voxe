#pragma once
#include "WindowContainer.h"
#include "Graphics/GraphicEngine.h"
#include "Timer.h"

class Engine : WindowContainer {
public:
	Engine();
	~Engine();
	
	bool init(HINSTANCE hInstance, std::wstring window_title, std::wstring window_class, int width, int height);
	bool processMessages();
	void update();
private:
	GraphicEngine gfxEngine_;
	Timer timer_;
};

