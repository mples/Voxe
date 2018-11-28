#include "InputMapper.h"



InputMapper::InputMapper() {
}


InputMapper::~InputMapper() {
}

void InputMapper::clear() {
}

void InputMapper::setRawButtons(RawButton button) {

}

void InputMapper::setRawAxis(RawAxis axis) {
}

void InputMapper::addCallback(Callback callback) {
}

void InputMapper::dispatch() {
}

void InputMapper::pushBackContext(InputContext * input_context) {
	auto found = std::find(activeContexts_.begin(), activeContexts_.end(), input_context);
	if (found == activeContexts_.end()) {
		activeContexts_.push_back(input_context);
	}
}

void InputMapper::removeContext(InputContext * input_context) {
}

void InputMapper::addFrontContext(InputContext * input_context) {
	auto found = std::find(activeContexts_.begin(), activeContexts_.end(), input_context);
	if (found == activeContexts_.end()) {
		activeContexts_.push_front(input_context);
	}
}

void InputMapper::moveFrontContext(InputContext * input_context) {
}

void InputMapper::moveBackContext(InputContext * input_context) {
}

void InputMapper::mapButtonToAction(RawButton button, Action & out) {
}

void InputMapper::mapButtonToState(RawButton button, State & out) {
}
