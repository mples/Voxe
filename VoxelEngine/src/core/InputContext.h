#pragma once
#include <unordered_map>
#include "RawInput.h"
#include "input_const.h"

class InputContext {
public:
	InputContext();
	~InputContext();
	InputContext(const InputContext& other) = default;
	InputContext& operator=(const InputContext& other) = default;

	bool mapButtonToAction(RawButton button, Action& out);
	bool mapButtonToState(RawButton button, State& out);
	bool mapAxisToRange(RawAxis axis, Range& out);
private:
	std::unordered_map<RawButton, Action> actionMap_;
	std::unordered_map<RawButton, State> stateMap_;
	std::unordered_map<RawAxis, Range> rangeMap_;
};

