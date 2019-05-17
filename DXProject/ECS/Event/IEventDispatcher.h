#pragma once

#include "EventDelegate.h"

class IEventDispatcher {
public:
	virtual ~IEventDispatcher() {};

	virtual void dispatch(IEvent* event) = 0;

	virtual void addEventCallback(IEventDelegate* event_delegate) = 0;

	virtual void removeEventDelegate(IEventDelegate* event_delegate) = 0;

	virtual unsigned int getEventCallbacksCount() = 0;
};