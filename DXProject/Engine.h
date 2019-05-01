#pragma once
#include "WindowContainer.h"
#include "Graphics/GraphicEngine.h"
#include "Utilities/Timer.h"

#include "Input/InputManager.h"
#include "Stage/StageManager.h"
#include "Utilities/JobSystem.h"

class Engine : WindowContainer {
public:
	Engine();
	~Engine();
	
	bool init(HINSTANCE hInstance, std::wstring window_title, std::wstring window_class, int width, int height);
	bool processMessages();
	void update();
private:
	//GraphicEngine gfxEngine_;
	Timer timer_;

	InputContext engineContext_;
};

