#pragma once

class IAllocator {
public:
	virtual void* allocate(size_t size) = 0;
	virtual void free(void* pointer, size_t size) = 0;
};