#pragma once
#include "DefaultAllocator.h"
#include <stdexcept>
#include <new>


template<typename T>
class PoolAllocator {
public:
	PoolAllocator(size_t initial_capacity = 1024, size_t max_block_length = 1000000) : 
		firstDeleted_(nullptr), elementsInBlock_(0), blockCapacity_(initial_capacity), firstBlock_(initial_capacity), maxBlockLength_(max_block_length) {
		if (max_block_length < 1) {
			throw std::invalid_argument("Capacity must be at least 1");
		}

		currentMemory_ = firstBlock_.memory_;
		lastBlock_ = &firstBlock_;
	}

	template<class T, class ...P>
	T* allocate(P&&... param) {
		if (firstDeleted_) {
			T* result = firstDeleted_;
			firstDeleted_ = *((T **)firstDeleted_);
			new(result) T(std::forward<P>(param)...);
			return result;
		}

		if (elementsInBlock_ > blockCapacity_) {
			allocateNewNode();
		}
		char * address = (char*)currentMemory_;
		address += elementsInBlock_ * itemSize_;
		T* result = new(address) T(std::forward<P>(param)...);
		elementsInBlock_++;
		return result;

	}

	void freeMemory(T* object) {
		object->~T();
		*((T **)object) = firstDeleted_;
		firstDeleted_ = object;
	}

private:
	struct MemoryBlock {
		void * memory_;
		size_t capacity_;
		MemoryBlock* nextBlock_;
		
		MemoryBlock(size_t capacity) {
			if (capacity < 1) {
				throw std::invalid_argument("Capacity must be at least 1");
			}
			memory_ = ::operator new(itemSize_ * capacity, ::std::nothrow);
			if (memory_ == nullptr) {
				throw std::bad_alloc();
			}
			capacity_ = capacity_;
			nextBlock_ = nullptr;
		}
		~MemoryBlock() {
			::operator delete(memory_);
		}
	};

	void* currentMemory_;
	T* firstDeleted_;
	size_t elementsInBlock_;
	size_t blockCapacity_;
	MemoryBlock firstBlock_;
	MemoryBlock* lastBlock_;
	size_t maxBlockLength_;

	static const size_t itemSize_;

	PoolAllocator(const PoolAllocator<T>& other) = delete;
	PoolAllocator& operator= (const PoolAllocator<T>& other) = delete;
	
	void allocateNewNode() {
		size_t size = elementsInBlock_;
		if (size >= maxBlockLength_) {
			size = maxBlockLength_;
		}
		else {
			size *= 2;
			if (size < elementsInBlock_) {
				throw std::overflow_error("Size overflow");
			}
			if (size <= maxBlockLength_) {
				size = maxBlockLength_;
			}
		}

		MemoryBlock *new_block = new MemoryBlock(size);
		lastBlock_->nextBlock_ = new_block;
		lastBlock_ = new_block;

		currentMemory_ = new_block->memory_;
		elementsInBlock_ = 0;
		blockCapacity_ = size;
	}
public:
	class iterator : public std::iterator<std::forward_iterator_tag, T> {
		MemoryBlock* currentBlock_;
		MemoryBlock* firstBlock;
		size_t objectIndex_;

		iterator(MemoryBlock* first_block, size_t index = 0) : currentBlock_(firstBlock), currentBlock_(first_block), objectIndex_(index) {

		}
		iterator(MemoryBlock* first_block, MemoryBlock* curr_block, size_t index = 0) : currentBlock_(firstBlock), currentBlock_(curr_block), objectIndex_(index) {

		}

		inline iterator& operator++() {
			if (currentBlock_->nextBlock == nullptr && objectIndex_ > elementsInBlock_) {
				
				//end ?
				return *this;
			}
			objectIndex_++;

			if (size_t > currentBlock_->capcity_) {
				if (currentBlock_->nextBlock_ != nullptr) {
					currentBlock_ = currentBlock_->nextBlock_;
					objectIndex_ = 0;
				}
				else {
					//end ?
					return *this;
				}
			}
			return *this;
		}

		inline T& operator*() const {
			char * address = (char*)currentBlock_->memory_;
			address += objectIndex_ * itemSize_;
			return *(static_cast<T*>(address));
		}

		inline T* operator->() const {
			char * address = (char*)currentBlock_->memory_;
			address += objectIndex_ * itemSize_;
			return static_cast<T*>(address);
		}

		inline bool operator==(iterator& other) {
			return currentBlock_ == other.currentBlock_ && objectIndex_ == other.objectIndex_;
		}

		inline bool operator!=(iterator& other) {
			return currentBlock_ != other.currentBlock_ || objectIndex_ != other.objectIndex_;
		}
	};

	inline iterator begin() {
		return iterator(&firstBlock_);
	}

	inline iterator end() {
		return iterator(&firstBlock_, lastBlock_, elementsInBlock_);
	}
};

template<typename T>
const size_t PoolAllocator<T>::itemSize_ = ((sizeof(T) + sizeof(void *) - 1) / sizeof(void *)) * sizeof(void *);