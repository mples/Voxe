#pragma once
#include "WindowContainer.h"
#include "Graphics/GraphicEngine.h"
#include "Utilities/Timer.h"

#include "Input/InputManager.h"
#include "Stage/StageManager.h"
#include "Utilities/JobSystem.h"
#include "ECS/EntityManager.h"
#include "ECS/ComponentManager.h"
#include "ECS/SystemManager.h"
#include "ECS/Event/EventHandler.h"

#define ENGINE Engine::getInstance()

class Engine : public Singleton<Engine>{
public:
	Engine();
	~Engine();
	
	bool init(HINSTANCE hInstance, std::wstring window_title, std::wstring window_class, int width, int height);
	bool processMessages();
	void update();

	EventHandler * getEventHandler();

	template<class EventType, class... ARGS>
	void sendEvent(ARGS&& ... args) {
		eventHandler_.send<EventType>(std::forward<ARGS>(args)...);
	}

	double getGlobalTime();
private:
	WindowContainer windowContainer_;
	//GraphicEngine gfxEngine_;
	Timer timer_;
	Timer globalTimer_;
	
	InputContext engineContext_;

	EntityManager entityManager_;
	ComponentManager componentManager_;
	SystemManager systemManager_;
	EventHandler eventHandler_;
};

