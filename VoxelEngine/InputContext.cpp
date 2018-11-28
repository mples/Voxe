#include "InputContext.h"



InputContext::InputContext() {
}


InputContext::~InputContext() {
}

bool InputContext::mapButtonToAction(RawButton button, Action & out) {
	auto found = actionMap_.find(button);

	if(found == actionMap_.end())
		return false;
	
	out = found->second;
	return true;
}

bool InputContext::mapButtonToState(RawButton button, State & out) {
	auto found = stateMap_.find(button);

	if (found == stateMap_.end())
		return false;

	out = found->second;
	return true;
}

bool InputContext::mapAxisToRange(RawAxis axis, Range & out) {
	auto found = rangeMap_.find(axis);
	if(found == rangeMap_.end())
		return false;

	out = found->second;
	return true;
}
