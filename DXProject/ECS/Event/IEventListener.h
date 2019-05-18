#pragma once
#include <list>
#include <functional>

#include "EventHandler.h"
#include "EventDelegate.h"
#include "../../Engine.h"

class IEventListener {
public:
	IEventListener(EventHandler * event_handler = ENGINE.getEventHandler()) : eventHandler_(event_handler) {}
	virtual ~IEventListener() = 0;

	template<class EventType>
	void registerEventCallback(std::function<void(const EventType * e)> callback) {
		IEventDelegate* callback = new EventDelegate<EventType>(callback);
		registeredCallbacks_.push_back(callback);
		eventHandler_->removeEventCallback<EventType>(callback);
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