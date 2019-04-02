#pragma once

#include  "ErrorLogger.h"

template <typename T>
class Singleton {
public:
	Singleton() {
		if (instance_ == nullptr) {
			instance_ = static_cast<T*>(this);
		}
		else {
			ErrorLogger::log(L"Error: Singleton already created.");
		}
	}
	virtual ~Singleton() {
		instance_ = nullptr;
	}

	static T & getInstance() {
		return *instance_;
	}

	static void createInstance() {
		if (!instance_) {
			instance_ = new T();
		}
	}

	static void removeInstance() {
		delete instance_;
	}

private:
	static T* instance_;
};

template <typename T> T* Singleton<T>::instance_ = nullptr;