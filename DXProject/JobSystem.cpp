#include "JobSystem.h"
#include <algorithm>


JobSystem::JobSystem() : threadsCount_(0), currentCount_(0) {
	finishedCount_.store(0);

	unsigned int thread_count = std::thread::hardware_concurrency();

	threadsCount_ = std::max(1u, thread_count);

	for (unsigned int thread_id = 0; thread_id < threadsCount_; thread_id++) {
		std::thread worker([&] {
			Job job;

			while (true) {
				if (jobQueue_.pop(job)) {
					job();
					finishedCount_.fetch_add(1);
				}
				else {
					std::unique_lock<std::mutex> lock(wakeMutex_);
					wakeCondition_.wait(lock);
				}
			}
		});

		worker.detach();
	}
}


JobSystem::~JobSystem() {
}

void JobSystem::initialize() {
	finishedCount_.store(0);

	unsigned int thread_count = std::thread::hardware_concurrency();

	threadsCount_ = std::max(1u, thread_count);

	for (unsigned int thread_id; thread_id < threadsCount_; thread_id++) {
		std::thread worker([&] {
			Job job;

			while (true) {
				if (jobQueue_.pop(job)) {
					job();
					finishedCount_.fetch_add(1);
				}
				else {
					std::unique_lock<std::mutex> lock(wakeMutex_);
					wakeCondition_.wait(lock);
				}
			}
		});

		worker.detach();
	}
}

void JobSystem::execute(const Job& job) {
	currentCount_ += 1;

	while (!jobQueue_.push(job)) {
		poll();
	}
	wakeCondition_.notify_one();
}

void JobSystem::executeBatch(unsigned int job_count, unsigned int group_size, const std::function<void(DispatchingArgs args)>& job) {
	if (job_count == 0 || group_size == 0) {
		return;
	}

	//calculate amount of jobs to dispatch
	const unsigned int group_count = (job_count + group_size - 1) / group_size;

	currentCount_ += group_count;

	for (unsigned int group_index = 0; group_index < group_count; group_index++) {

		Job group_job = [&]() {
			unsigned int job_group_offset = group_index * group_size;
			unsigned int job_group_end = std::min(job_group_offset + group_size, job_count);

			DispatchingArgs args;
			args.groupIndex_ = group_index;

			for (unsigned int i = 0; i < job_group_offset; i++) {
				args.jobIndex_ = i;
				job(args);
			}
		};

		while (!jobQueue_.push(group_job)) {
			poll();
		}
		wakeCondition_.notify_one();
	}

}

bool JobSystem::isBusy() {
	return finishedCount_.load() < currentCount_;
}

void JobSystem::wait() {
	while (isBusy()) {
		poll();
	}
}

inline void JobSystem::poll() {
	wakeCondition_.notify_one();
	std::this_thread::yield();
}
