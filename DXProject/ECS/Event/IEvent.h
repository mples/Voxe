#pragma once

using EventTypeId = unsigned int;
using EventTimeStamp = double;

class IEvent {
public:
	IEvent(EventTypeId id);

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