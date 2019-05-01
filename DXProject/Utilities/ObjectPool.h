#pragma once
#include "Singleton.h"
#include <array>
#include<list>
#include <mutex>

template <typename T, unsigned int size>
class ObjectPool {
public:
	ObjectPool();
	T* create();
	void release(T* obj);

private:
	std::array<T, size> objects_;
	std::list<T*> freeObjects_;
	std::mutex mutex_;
};

template<typename T, unsigned int size>
ObjectPool<T, size>::ObjectPool() : objects_{} {
	for (T& obj : objects_) {
		freeObjects_.push_back(&obj);
	}
}

template<typename T, unsigned int size>
T * ObjectPool<T, size>::create() {
	std::lock_guard <std::mutex> lock(mutex_);
	if (freeObjects_.empty()) {
		return NULL;
	}
	else {
		T* obj = freeObjects_.back();
		freeObjects_.pop_back();
		return obj;
	}
}

template<typename T, unsigned int size>
void ObjectPool<T, size>::release(T * obj) {
	std::lock_guard <std::mutex> lock(mutex_);
	obj->initialize();
	freeObjects_.push_back(obj);
}
