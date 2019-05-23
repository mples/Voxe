#pragma once
#include <list>
#include <algorithm>
#include <cassert>
#include "IEventDispatcher.h"

template<class T>
class EventDispatcher : public IEventDispatcher {
public:
	EventDispatcher() : locked_(false) {}

	// Inherited via IEventDispatcher
	virtual void dispatch(IEvent * event) override {
		locked_ = true;

		for (IEventDelegate* dele : pendingDeletions_) {

			auto found = std::find_if(eventCallbacks_.begin(), eventCallbacks_.end(), [&](const IEventDelegate* other) {
				return dele == other;
			});
			if (found != eventCallbacks_.end()) {
				IEventDelegate* to_del = *found;
				eventCallbacks_.erase(found);

				delete to_del;
				to_del = nullptr;
			}
		}
		pendingDeletions_.clear();

		for (IEventDelegate* dele : eventCallbacks_) {
			assert(dele != nullptr);
			dele->invoke(event);
		}
		locked_ = false;

	}
	virtual void addEventCallback(IEventDelegate * event_delegate) override {
		eventCallbacks_.push_back(event_delegate);
	}
	virtual void removeEventDelegate(IEventDelegate * event_delegate) override {
		if (!locked_) {
			auto found = std::find_if(eventCallbacks_.begin(), eventCallbacks_.end(), [&](const IEventDelegate* other) {
				return event_delegate == other;
			});
			if (found != eventCallbacks_.end()) {
				IEventDelegate* to_del = *found;
				eventCallbacks_.erase(found);

				delete to_del;
				to_del = nullptr;
			}
		}
		else {
			auto found = std::find_if(eventCallbacks_.begin(), eventCallbacks_.end(), [&](const IEventDelegate* other) {
				return event_delegate == other;
			});
			if (found != eventCallbacks_.end()) {
				pendingDeletions_.push_back(event_delegate);
			}
		}
	}
	virtual unsigned int getEventCallbacksCount() override {
		return static_cast<unsigned int>(eventCallbacks_.size());
	}

private:
	std::list<IEventDelegate*> eventCallbacks_;
	std::list<IEventDelegate*> pendingDeletions_;
	bool locked_;
};