#pragma once
#include "Component.h"
#include "InputContext.h"
#include "Input.h"

class InputComponent : public Component{
public:
	InputComponent();
	InputComponent(InputContext* input_context, std::vector<std::pair<InputCallback, InputCallbackPriority>>& callbacks);
	~InputComponent();

	void activate();
	void deactivate();

	void addCallback(std::pair<InputCallback, InputCallbackPriority>& callback);
	void removeCallback(std::pair<InputCallback, InputCallbackPriority>& callback);

	void setContext(InputContext* input_context);

	// Inherited via Component
	virtual void update(float dt) override;
private:
	InputContext* inputContext_;
	std::vector<std::pair<InputCallback, InputCallbackPriority>> callbacks_;

	void addCBsToMapper(std::vector<std::pair<InputCallback, InputCallbackPriority>>& callbacks);
	void addCBToMapper(std::pair<InputCallback, InputCallbackPriority>& callback);
	void removeCBsFromMapper(std::vector<std::pair<InputCallback, InputCallbackPriority>>& callbacks);
	void removeCBFromMapper(std::pair<InputCallback, InputCallbackPriority>& callback);
};

