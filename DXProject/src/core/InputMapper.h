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
	std::map<Range, double> ranges_;

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

enum InputCallbackPriority {
	HIGH = 3,
	MEDIUM = 2,
	LOW = 1
};
using InputCallback = std::function<void(MappedInput& input)>;
class InputMapper {
	
public:
	InputMapper();
	~InputMapper();

//Raw input
	void clear();
	void setRawButtons(RawButton& button);
	void setRawAxis(RawAxis& axis);

//Callback
	void addCallback(InputCallback callback, InputCallbackPriority priority);
	void removeCallback(InputCallback callback);
	void dispatch();
//Context
	void pushBackContext(InputContext* input_context);
	void removeContext(InputContext* input_context);
	void addFrontContext(InputContext* input_context);
	void moveFrontContext(InputContext* input_context);
	void moveBackContext(InputContext* input_context);
private:
	bool mapButtonToAction(RawButton button, Action& out);
	bool mapButtonToState(RawButton button, State& out);

	std::list<InputContext*> activeContexts_;
	MappedInput currentInput_;
	std::multimap<int, InputCallback> callbacks_;
};

