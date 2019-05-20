#pragma once
#include <list>
#include <functional>

#include "EventHandler.h"
#include "EventDelegate.h"

class IEventListener {
public:
	IEventListener(EventHandler * event_handler ) : eventHandler_(event_handler) {}
	virtual ~IEventListener() {};

	template<class EventType>
	IEventDelegate* registerEventCallback(std::function<void(const EventType * e)> callback) {
		IEventDelegate* deleg = new EventDelegate<EventType>(callback);
		registeredCallbacks_.push_back(deleg);
		eventHandler_->addEventCallback<EventType>(deleg);
		return deleg;
	}

	void unregisterEventCallback(IEventDelegate* deleg) {
		registeredCallbacks_.remove(deleg);
		eventHandler_->removeEventCallback(deleg);
	}

	void unregistelAllEventCallbacks() {
		for (IEventDelegate* deleg : registeredCallbacks_) {
			eventHandler_->removeEventCallback(deleg);
		}
		registeredCallbacks_.clear();
	}
private:
	std::list<IEventDelegate*> registeredCallbacks_;
	EventHandler* eventHandler_;
};