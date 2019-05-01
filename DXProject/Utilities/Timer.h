#pragma once
#include <chrono>

class Timer {
public:
	Timer();
	~Timer();
	
	bool start();
	bool stop();
	void restart();
	double getTimePassedMilisec();
private:
	bool isRunning_;
	std::chrono::time_point<std::chrono::steady_clock> start_;
	std::chrono::time_point<std::chrono::steady_clock> stop_;
};

