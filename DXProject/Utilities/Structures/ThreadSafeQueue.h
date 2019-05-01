#pragma once
#include <mutex>

template<typename T, size_t size>
class ThreadSafeQueue {
public:
	inline bool push(const T& item) {
		std::lock_guard<std::mutex> lock(mutex_);
		size_t next = (head_ + 1) % size;

		if (next != tail_) {
			data_[head_] = item;
			head_ = next; 
			return true;
		}
		else {
			return false;
		}
	}

	inline bool pop(T& item) {
		std::lock_guard<std::mutex> lock(mutex_);

		if (tail_ != head_) {
			item = data_[tail_];
			tail_ = (tail_ + 1) % size;
			return true;
		}
		else {
			return false;
		}
	}

private:
	T data_[size];
	size_t head_ = 0;
	size_t tail_ = 0;
	std::mutex mutex_;
};

