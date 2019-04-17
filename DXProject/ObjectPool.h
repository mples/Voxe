#pragma once
#include "Singleton.h"
#include <array>
#include<list>


template <typename T, unsigned int size>
class ObjectPool {
public:
	ObjectPool();
	T* create();
	void release(T* obj);

private:
	std::array<T, size> objects_;
	std::list<T*> freeObjects_;
};

template<typename T, unsigned int size>
ObjectPool<T, size>::ObjectPool() : objects_{} {
	for (T& obj : objects_) {
		freeObjects_.push_back(&obj);
	}
}

template<typename T, unsigned int size>
T * ObjectPool<T, size>::create() {
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
	obj->initialize();
	freeObjects_.push_back(obj);
}
