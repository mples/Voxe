#include "Timer.h"



Timer::Timer() : isRunning_(false) {
	start_ = std::chrono::high_resolution_clock::now();
	stop_ = std::chrono::high_resolution_clock::now();
}


Timer::~Timer() {
}

bool Timer::start() {
	if (isRunning_) {
		return false;
	}
	else {
		start_ = std::chrono::high_resolution_clock::now();
		isRunning_ = true;
		return true;
	}
}

bool Timer::stop() {
	if (!isRunning_) {
		return false;
	}
	else {
		stop_ = std::chrono::high_resolution_clock::now();
		isRunning_ = false;
		return true;
	}

}

void Timer::restart() {
	isRunning_ = true;
	start_ = std::chrono::high_resolution_clock::now();

}

double Timer::getTimePassedMilisec() {
	if (isRunning_) {
		auto passed = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - start_);
		return passed.count();
	}
	else {
		auto passed = std::chrono::duration<double, std::milli>(stop_ - start_);
		return passed.count();
	}
}
