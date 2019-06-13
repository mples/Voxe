#include "EventHandler.h"
#include <cassert>

EventHandler::EventHandler() {
}

EventHandler::~EventHandler() {
}

void EventHandler::clearEventsBuffer() {
	for (IEvent* e : eventsBuffer_) {
		delete e;
	}
	eventsBuffer_.clear();


}

void EventHandler::clearEventsDispatcherMap() {
	eventDispatchersMap_.clear();
}

void EventHandler::dispatchEvents() {
	for (IEvent * event : eventsBuffer_) {
		assert(event != nullptr);
		auto found = eventDispatchersMap_.find(event->getTypeId());
		if (found != eventDispatchersMap_.end()) {
			found->second->dispatch(event);
		}
	}
	clearEventsBuffer();


}

void EventHandler::removeEventCallback(IEventDelegate * event_delegate) {
	EventTypeId event_type_id = event_delegate->getEventTypeId();
	std::lock_guard<std::mutex> lock(mutex_);

	auto delegate_it = eventDispatchersMap_.find(event_type_id);
	if (delegate_it != eventDispatchersMap_.end()) {
		delegate_it->second->removeEventDelegate(event_delegate);
	}
}
