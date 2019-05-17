#pragma once
#include "IEvent.h"
#include "../TypeId.h"

using EventTypeId = unsigned int;

template<class T>
class Event : public IEvent {
public:
	Event() : IEvent(TYPE_ID) {}

	static const EventTypeId TYPE_ID;
private:
};

template<class T>
const EventTypeId Event<T>::TYPE_ID{ typeid(T)::hash_code() };