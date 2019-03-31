#pragma once
#include <unordered_map>
#include "RawInput.h"
#include "input_const.h"
#include "RangeConverter.h"

class InputContext {
public:
	InputContext();
	~InputContext();
	InputContext(const InputContext& other) = default;
	InputContext& operator=(const InputContext& other) = default;

	bool mapButtonToAction(RawButton button, Action& out);
	bool mapButtonToState(RawButton button, State& out);
	bool mapAxisToRange(RawAxis axis, Range& out);

	void addActionMapping(Action action, RawButton button);
	void addStateMapping(State state, RawButton button);
	void addRangeMapping(Range range, RawAxis axis);

	void addRangeConverter(Range range, RangeConverter range_converter);

	double convert(Range range, double value);
private:
	//TODO change to keycod insted of RawButton objects
	std::unordered_map<RawButton, Action> actionMap_;
	std::unordered_map<RawButton, State> stateMap_;
	std::unordered_map<RawAxis, Range> rangeMap_;

	std::unordered_map<Range, RangeConverter> rangeConverters_;
};

