#pragma once
#include <unordered_map>
#include "RawInput.h"
#include "input_const.h"
#include "RangeConverter.h"
#include "Keyboard/KeyboardEvent.h"

class InputContext {
public:
	InputContext();
	~InputContext();
	InputContext(const InputContext& other) = default;
	InputContext& operator=(const InputContext& other) = default;

	bool mapButtonToAction(KeyboardEvent button, Action& out);
	bool mapButtonToState(KeyboardEvent button, State& out);
	bool mapAxisToRange(RawAxis axis, Range& out);

	void addActionMapping(Action action, KeyboardEvent button);
	void addStateMapping(State state, KeyboardEvent button);
	void addRangeMapping(Range range, RawAxis axis);

	void addRangeConverter(Range range, RangeConverter range_converter);

	double convert(Range range, double value);
private:
	std::unordered_map<KeyboardEvent, Action> actionMap_;
	std::unordered_map<KeyboardEvent, State> stateMap_;
	std::unordered_map<RawAxis, Range> rangeMap_;

	std::unordered_map<Range, RangeConverter> rangeConverters_;
};

