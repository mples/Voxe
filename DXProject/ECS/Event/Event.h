#pragma once
#include "IEvent.h"
#include "../TypeId.h"

Id_t TypeId<IEvent>::count_ = 0u;

template<class T>
class Event : public IEvent {
public:
	Event() : IEvent(TYPE_ID) {}

	static const EventTypeId TYPE_ID;
};

template<class T>
const EventTypeId Event<T>::TYPE_ID = TypeId<IEvent>::get<T>();