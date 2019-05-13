#pragma once
#include "IAllocator.h"
#include <new>

class DefaultAllocator : public IAllocator {
	// Inherited via IAllocator
	virtual void * allocate(size_t size) override;
	virtual void free(void * pointer, size_t size) override;
};