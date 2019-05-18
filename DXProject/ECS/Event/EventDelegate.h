#pragma once
#include "IEventDelegate.h"
#include <functional>


template<class EventType>
class EventDelegate : public IEventDelegate {
public:
	using EventCallback = std::function<void(const EventType* event)>;

	EventDelegate( EventCallback& callback) : callback_(callback) {}

	// Inherited via IEventDeleagate
	virtual void invoke(const IEvent * const e) override {
		callback_(reinterpret_cast<EventType*>(e));
	}
	virtual EventDelegateId getId() const override {
		static const EventDelegateId EVENT_DELEGATE_ID {typeid(EventCallback).hash_code()};
		return EVENT_DELEGATE_ID;
	}
	virtual EventTypeId getEventTypeId() const override {
		return EventType::TYPE_ID;
	}
	virtual bool operator==(const IEventDelegate * other) override {
		if (getEventTypeId() != other->getEventTypeId()) {
			return false;
		}
		EventDelegate* event_delegate_other = dynamic_cast<EventDelegate*>(other);
		if (event_delegate_other == nullptr) {
			return false;
		}

		return callback_ == event_delegate_other->callback_;
	}

	virtual bool operator!=(const IEventDelegate * other) override {
		if (getEventTypeId() != other->getEventTypeId()) {
			return true;
		}
		EventDelegate* event_delegate_other = dynamic_cast<EventDelegate*>(other);
		if (event_delegate_other == nullptr) {
			return true;
		}

		return callback_ != event_delegate_other->callback_;
	}

	virtual IEventDelegate * clone() override {
		return new EventDelegate(callback_);
	}
private:

	EventCallback callback_;
};