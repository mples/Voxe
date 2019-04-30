#pragma once
#include <functional>
#include <atomic>

#include "ThreadSafeQueue.h"
#include "Singleton.h"

struct DispatchingArgs {
	unsigned int jobIndex_;
	unsigned int groupIndex_;
};

typedef std::function<void()> Job;

class JobSystem : public Singleton<JobSystem> {
public:
	JobSystem();
	~JobSystem();

	void initialize();
	void execute(const Job& job);
	void executeBatch(unsigned int job_count, unsigned int group_size, const std::function<void(DispatchingArgs args)>& job);

	bool isBusy();
	void wait();


	ThreadSafeQueue<Job, 256> jobQueue_;
	unsigned int currentCount_;
	std::atomic<unsigned int> finishedCount_;
	unsigned int threadsCount_;
	std::condition_variable wakeCondition_;
	std::mutex wakeMutex_;
private:
	inline void poll();

};

#define JOB_SYSTEM JobSystem::getInstance()
