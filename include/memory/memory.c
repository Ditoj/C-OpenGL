#include "memory/memory.h"

#include <stdlib.h>

void* reallocate(void* array, size_t size) {
	if(size == 0) {
		free(array);
		return NULL;
	}

	void* result = realloc(array, size);
	if(result == NULL) exit(1);

	return result;
}