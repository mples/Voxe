#include "InputComponent.h"
#include <functional>
#include <vector>

InputComponent::InputComponent() : Component(ComponentType::InputComponent) {
}

InputComponent::InputComponent(InputContext* input_context, std::vector<std::pair<InputCallback, InputCallbackPriority>>& callbacks ) : Component(ComponentType::InputComponent) {
	inputContext_ = input_context;
	callbacks_ = callbacks;
}


InputComponent::~InputComponent() {
	if (inputContext_ != nullptr) {
		Input::getInstance().getMapper().removeContext(inputContext_);
		delete inputContext_;
	}
}

void InputComponent::activate() {
	Input::getInstance().getMapper().addFrontContext(inputContext_);
	addCBsToMapper(callbacks_);
}

void InputComponent::deactivate() {
	Input::getInstance().getMapper().removeContext(inputContext_);
	removeCBsFromMapper(callbacks_);

}

void InputComponent::addCallback(std::pair<InputCallback, InputCallbackPriority>& callback) {
	/*auto found = std::find(callbacks_.begin(), callbacks_.end(), callback);
	if (found == callbacks_.end()) {
		callbacks_.push_back(callback);
	}*/
	//TODO find way to manage callbacks better
	callbacks_.push_back(callback);
}

void InputComponent::removeCallback(std::pair<InputCallback, InputCallbackPriority>& callback) {
	/*auto found = std::find(callbacks_.begin(), callbacks_.end(), callback);
	if (found != callbacks_.end()) {
		callbacks_.erase(found);
		removeCBFromMapper(callback);
	}*/
	//TODO find way to delete callbacks 
}

void InputComponent::setContext(InputContext * input_context) {
	if (inputContext_ != nullptr) {
		Input::getInstance().getMapper().removeContext(inputContext_);
		delete inputContext_;
	}
	inputContext_ = input_context;
}

void InputComponent::addCBsToMapper(std::vector<std::pair<InputCallback, InputCallbackPriority>>& callbacks) {
	for (auto callback : callbacks) {
		addCBToMapper(callback);
	}
}

void InputComponent::addCBToMapper(std::pair<InputCallback, InputCallbackPriority>& callback) {
	Input::getInstance().getMapper().addCallback(callback.first, callback.second);
}

void InputComponent::removeCBsFromMapper(std::vector<std::pair<InputCallback, InputCallbackPriority>>& callbacks) {
	for (auto callback : callbacks_) {
		removeCBFromMapper(callback);
	}
}

void InputComponent::removeCBFromMapper(std::pair<InputCallback, InputCallbackPriority>& callback) {
	Input::getInstance().getMapper().removeCallback(callback.first);
}

void InputComponent::update(float dt) {
}
