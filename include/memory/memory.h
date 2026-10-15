#ifndef tuph_memory_h
#define tuph_memory_h

#include "common/common.h"

#define ALLOCATE(type, count) \
	(type *) reallocate(NULL, sizeof(type) * (count));

#define GROW_CAPACITY(capacity) \
	((capacity) < 8 ? 8 : (capacity) * 2)

#define GROW_ARRAY(type, array, newCapacity) \
	(type *) reallocate(array, sizeof(type) * (newCapacity))

#define FREE_ARRAY(array) \
	reallocate(array, 0)

#define FREE(pointer) reallocate(pointer, 0)

void* reallocate(void* array, size_t size);

#endif