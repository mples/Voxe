#include "DefaultAllocator.h"

void * DefaultAllocator::allocate(size_t size) {
	return ::operator new(size, ::std::nothrow);
}

void DefaultAllocator::free(void * pointer, size_t size) {
	::operator delete(pointer);
}
