#pragma once
#include <set>
#include <map>
#include <list>
#include <functional>


#include "input_const.h"
#include "RawInput.h"
#include "InputContext.h"

struct MappedInput {
	std::set<Action> actions_;
	std::set<State> states_;
	std::map<Range, float> ranges_;

	void eatAction(Action action) {
		actions_.erase(action);
	}
	void eatState(State state) {
		states_.erase(state);
	}
	void eatRange(Range range) {
		auto found = ranges_.find(range);
		if (found != ranges_.end()) {
			ranges_.erase(found);
		}
	}
};

class InputMapper {
	using Callback = std::function<void(MappedInput& input)>;
public:
	InputMapper();
	~InputMapper();

//Raw input
	void clear();
	void setRawButtons(RawButton button);
	void setRawAxis(RawAxis axis);

//Callback
	void addCallback(Callback callback);
	void dispatch();
//Context
	void pushBackContext(InputContext* input_context);
	void removeContext(InputContext* input_context);
	void addFrontContext(InputContext* input_context);
	void moveFrontContext(InputContext* input_context);
	void moveBackContext(InputContext* input_context);
private:
	void mapButtonToAction(RawButton button, Action& out);
	void mapButtonToState(RawButton button, State& out);

	std::list<InputContext*> activeContexts_;

};

