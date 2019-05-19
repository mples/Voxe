#include "IEvent.h"
#include "../../Engine.h"

IEvent::IEvent(EventTypeId id) : typeId_(id) {
	timeStamp_ = ENGINE.getGlobalTime();
}