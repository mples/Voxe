#pragma once

using EventTypeId = unsigned int;
using EventTimeStamp = float;

class IEvent {
public:
	IEvent(EventTypeId id) : typeId_(id) {
		//TODO set time created as current
	}

	inline const EventTypeId getTypeId() {
		return typeId_;
	}

	inline const EventTimeStamp getTimeStamp() {
		return timeStamp_;
	}
private:

	EventTypeId typeId_;
	EventTimeStamp timeStamp_;
};