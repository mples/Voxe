#pragma once
#include <unordered_map>
#include <functional>
#include <iostream>

#include "../Utilities/Singleton.h"
#include "InputMapper.h"

#include "Keyboard/KeyboardManager.h"
#include "Mouse/MouseManager.h"

class InputManager : public Singleton<InputManager> {
public:
	InputManager();

	void processInput(float dt);
	//Callback
	void addCallback(InputCallback callback, InputCallbackPriority priority);
	void removeCallback(InputCallback callback);
	//Context
	void pushBackContext(InputContext* input_context);
	void removeContext(InputContext* input_context);
	void addFrontContext(InputContext* input_context);
	void moveFrontContext(InputContext* input_context);
	void moveBackContext(InputContext* input_context);
private:
	static float roundDiff(float value);
	void dispatchEvents();
	InputMapper mapper_;

};

#define INPUT InputManager::getInstance()
