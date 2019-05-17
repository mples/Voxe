#pragma once
#include <list>
#include <functional>

#include "EventDelegate.h"

class IEventListener {
public:
	IEventListener();
	virtual ~IEventListener();

	template<class EventType>
	void registerEventCallback(std::function<void(const EventType * e)> callback) {
		IEventDelegate* = new EventDelegate<EventType>(callback);

	}
private:
	std::list<IEventDelegate*> registeredCallbacks_;
};