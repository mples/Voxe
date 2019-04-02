#include "InputContext.h"



InputContext::InputContext() {
	//RawButton button(87, 17, 1, 0);
	/*RawButton button(87);
	actionMap_.insert(std::make_pair(button, Action::MOVE_FRONT) );
	RawAxis x_axis (AxisType::MOUSE_X);
	rangeMap_.insert(std::make_pair(x_axis, Range::LOOK_X));*/
}

InputContext::~InputContext() {}

bool InputContext::mapButtonToAction(KeyboardEvent button, Action & out) {
	auto found = actionMap_.find(button);

	if(found == actionMap_.end())
		return false;
	
	out = found->second;
	return true;
}

bool InputContext::mapButtonToState(KeyboardEvent button, State & out) {
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

void InputContext::addActionMapping(Action action, KeyboardEvent button) {
	actionMap_.insert(std::make_pair(button, action));
}

void InputContext::addStateMapping(State state, KeyboardEvent button) {
	stateMap_.insert(std::make_pair(button, state));
}

void InputContext::addRangeMapping(Range range, RawAxis axis) {
	rangeMap_.insert(std::make_pair(axis, range));
}

void InputContext::addRangeConverter(Range range, RangeConverter range_converter) {
	rangeConverters_.insert(std::make_pair(range, range_converter));
}

double InputContext::convert(Range range, double value) {
	//TODO templating
	auto found = rangeConverters_.find(range);
	if (found == rangeConverters_.end()) {
		return static_cast<double>(value);
	}

	return found->second.convert<double>(value);
}
