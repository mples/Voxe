#include "InputMapper.h"
#include "glfw3.h"


InputMapper::InputMapper() {
}


InputMapper::~InputMapper() {
}

void InputMapper::clear() {
}

void InputMapper::setRawButtons(RawButton& button) {
	if (button.action_ == GLFW_PRESS) {
		Action out_action;
		if (mapButtonToAction(button, out_action)) {
			currentInput_.actions_.insert(out_action);
			return;
		}
	}
	else if (button.action_ == GLFW_REPEAT) {
		State out_state;
		if (mapButtonToState(button, out_state)) {
			currentInput_.states_.insert(out_state);
			return;
		}
	}

}

void InputMapper::setRawAxis(RawAxis& axis) {
	for (auto it = activeContexts_.begin(); it != activeContexts_.end(); ++it) {
		Range range;
		if ((*it)->mapAxisToRange(axis, range)) {
			currentInput_.ranges_[range] = (*it)->convert(range, axis.value_);
			return;
		}
	}
}

void InputMapper::addCallback(Callback callback, Priority priority) {
	callbacks_.insert(std::make_pair(priority, callback) );
}

void InputMapper::dispatch() {
	for (auto it = callbacks_.begin(); it != callbacks_.end(); ++it) {
		it->second(currentInput_);
	}
}

void InputMapper::pushBackContext(InputContext * input_context) {
	auto found = std::find(activeContexts_.begin(), activeContexts_.end(), input_context);
	if (found == activeContexts_.end()) {
		activeContexts_.push_back(input_context);
	}
}

void InputMapper::removeContext(InputContext * input_context) {
	auto found = std::find(activeContexts_.begin(), activeContexts_.end(), input_context);
	if (found == activeContexts_.end()) {
		activeContexts_.erase(found);
	}
}

void InputMapper::addFrontContext(InputContext * input_context) {
	auto found = std::find(activeContexts_.begin(), activeContexts_.end(), input_context);
	if (found == activeContexts_.end()) {
		activeContexts_.push_front(input_context);
	}
}

void InputMapper::moveFrontContext(InputContext * input_context) {
	auto found = std::find(activeContexts_.begin(), activeContexts_.end(), input_context);
	if (found == activeContexts_.end()) {
		std::iter_swap(found, activeContexts_.begin());
	}
}

void InputMapper::moveBackContext(InputContext * input_context) {
	auto found = std::find(activeContexts_.begin(), activeContexts_.end(), input_context);
	if (found == activeContexts_.end()) {
		std::iter_swap(found, activeContexts_.rbegin());
	}
}

bool InputMapper::mapButtonToAction(RawButton button, Action & out) {
	for (auto it = activeContexts_.begin(); it != activeContexts_.end(); ++it) {
		if ((*it)->mapButtonToAction(button, out)) {
			return true;
		}
	}
	return false;
}

bool InputMapper::mapButtonToState(RawButton button, State & out) {
	for (auto it = activeContexts_.begin(); it != activeContexts_.end(); ++it) {
		if ((*it)->mapButtonToState(button, out)) {
			return true;
		}
	}
	return false;
}
