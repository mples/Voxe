#pragma once
#include <list>
#include <algorithm>

#include "IEventDispatcher.h"

template<class T>
class EventDispatcher : public IEventDispatcher {
public:
	EventDispatcher() {}

	// Inherited via IEventDispatcher
	virtual void dispatch(IEvent * event) override {
		for (IEventDeleagate* dele : eventCallbacks_) {
			assert(dele != nullptr);
			dele->invoke(event);
		}
	}
	virtual void addEventCallback(IEventDeleagate * event_delegate) override {
		eventCallbacks_.push_back(event_delegate);
	}
	virtual void removeEventDelegate(IEventDeleagate * event_delegate) override {
		auto found = std::find_if(eventCallbacks_.begin(), eventCallbacks_.end(), [&](const IEventDelegate* other) {
			return event_delegate == other;
		});
		if (found != eventCallbacks_.end()) {
			IEventDelegate* to_del = *found;
			eventCallbacks_.erase(found);

			delete to_del;
			to_del = nullptr;
		}
	}
	virtual unsigned int getEventCallbacksCount() override {
		eventCallbacks_.size();
	}

private:
	std::list<IEventDeleagate*> eventCallbacks_;
};