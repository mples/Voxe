#pragma once
#include "DefaultAllocator.h"
#include <stdexcept>

template<typename T, class MemoryAllocator = DefaultAllocator>
class PoolAllocator {
public:
	PoolAllocator(size_t initial_capacity = 1024, size_t max_block_length = 1000000) : 
		firstDeleted_(nullptr), countInNode_(0), nodeCapacity_(initial_capacity), firstNode_(initial_capacity), maxBlockLength_(max_block_length) {
		if (max_block_length < 1) {
			throw std::invalid_argument("Capacity must be at least 1");
		}

		nodeMemory_ = firstNode_.memory_;
		lastNode_ = &firstNode_;
	}

	template<class T, class ...P>
	T* allocate(P&&... param) {
		if (firstDeleted_) {
			T* result = firstDeleted_;
			firstDeleted_ = *((T **)firstDeleted_);
			new(result) T(std::forward<P>(param)...);
			return result;
		}

		if (countInNode_ > nodeCapacity_) {
			allocateNewNode();
		}
		char * address = (char*)nodeMemory_;
		address += countInNode_ * itemSize_;
		T* result = new(address) T(std::forward<P>(param)...);
		countInNode_++;
		return result;

	}

	void free(T* object) {
		object->~T();
		*((T **)object) = firstDeleted_;
		firstDeleted_ = object;
	}

private:
	struct Node {
		void * memory_;
		size_t capacity_;
		Node* nextNode_;
		
		Node(size_t capacity) {
			if (capacity < 1) {
				throw std::invalid_argument("Capacity must be at least 1");
			}
			memory_ = MemoryAllocator.allocate(itemSize_ * capacity);
			if (memory_ == nullptr) {
				throw std::bad_alloc();
			}
			capacity_ = capacity_;
			nextNode_ = nullptr;
		}
		~Node() {
			MemoryAllocator.free(memory_, itemSize_ * capacity_);
		}
	};

	void* nodeMemory_;
	T* firstDeleted_;
	size_t countInNode_;
	size_t nodeCapacity_;
	Node firstNode_;
	Node* lastNode_;
	rsize_t maxBlockLength_;

	static const size_t itemSize_;

	PoolAllocator(const PoolAllocator<T, MemoryAllocator>& other);
	void operator= (const PoolAllocator<T, MemoryAllocator>& other);
	
	void allocateNewNode() {
		size_t size = countInNode_;
		if (size >= maxBlockLength_) {
			size = maxBlockLength_;
		}
		else {
			size *= 2;
			if (size < countInNode_) {
				throw std::overflow_error("Size overflow");
			}
			if (size <= maxBlockLength_) {
				size = maxBlockLength_;
			}
		}

		Node *new_node = new Node(size);
		lastNode_->nextNode_ = new_node;
		lastNode_ = new_node;

		nodeMemory_ = new_node.memory_;
		countInNode_ = 0;
		nodeCapacity_ = size;
	}

};

template<typename T, class MemoryAllocator>
const size_t PoolAllocator<T, MemoryAllocator>::itemSize_ = ((sizeof(T) + sizeof(void *) - 1) / sizeof(void *)) * sizeof(void *);