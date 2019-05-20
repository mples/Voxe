#pragma once
#include "IEventDelegate.h"
#include <functional>
#include "../TypeId.h"

Id_t TypeId<IEventDelegate>::count_ = 0u;

template<class EventType>
class EventDelegate : public IEventDelegate {
public:
	using EventCallback = std::function<void(const EventType* event)>;

	EventDelegate( EventCallback& callback) : callback_(callback) {}

	// Inherited via IEventDeleagate
	virtual void invoke(const IEvent * const e) override {
		callback_(reinterpret_cast<const EventType*>(e));
	}
	virtual EventDelegateId getId() const override {
		static const EventDelegateId EVENT_DELEGATE_ID = TypeId<IEventDelegate>::get<EventType>();
		return EVENT_DELEGATE_ID;
	}
	virtual EventTypeId getEventTypeId() const override {
		return EventType::TYPE_ID;
	}
	
	virtual IEventDelegate * clone() override {
		return new EventDelegate(callback_);
	}
private:

	EventCallback callback_;
};