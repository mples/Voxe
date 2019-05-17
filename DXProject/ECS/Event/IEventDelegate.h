#pragma once
#include "IEvent.h"

using EventDelegateId = unsigned int;

class IEventDelegate {
public:
	virtual void invoke(const IEvent * const e) = 0;

	virtual EventDelegateId getId() const = 0;

	virtual EventTypeId getEventTypeId() const = 0;

	virtual bool operator==(const IEventDelegate* other) = 0;

	virtual bool operator!=(const IEventDelegate* other) = 0;

	virtual IEventDelegate* clone() = 0;
};