#pragma once

#include <unordered_map>

#include "EventDispatcher.h"

class EventHandler {
public:
	EventHandler();
	~EventHandler();

	void clearEventsBuffer();
	void clearEventsDispatcherMap();

	template<class EventType, class... ARGS>
	void send(ARGS&&... args) {
		static_assert(std::is_trivially_copyable<EventType>::value, "Event class is not tryvially copyable");

		EventType* e = new EventType(std::forward<ARGS>(args)...);

		eventsBuffer_.push_back(e);
	}

	void removeEventCallback(IEventDelegate * event_delegate);

	void dispatchEvents();
private:
	EventHandler(const EventHandler& other) = delete;
	EventHandler& operator=(const EventHandler& other) = delete;

	template<class EventType>
	void addEventCallback(IEventDelegate* event_delegate) {
		EventTypeId event_type_id = EventType::TYPE_ID;

		auto delegate_it = eventDispatchersMap_.find(event_type_id);
		if (delegate_it == eventDispatchersMap_.end()) {
			std::pair<EventTypeId, IEventDispatcher*> new_dispatcher(event_type_id, new EventDispatcher<EventType>());
			new_dispatcher.second->addEventCallback(event_delegate);
			eventDispatchersMap_.insert(new_dispatcher);
		}
		else {
			delegate_it->second->addEventCallback(event_delegate);
		}
	}


	std::unordered_map<EventTypeId, IEventDispatcher*> eventDispatchersMap_;
	std::vector<IEvent*> eventsBuffer_;

};