#include "hashmap/string_key.h"
#include "memory/memory.h"

#include <string.h>

static StringKey* allocateString(const char* chars, int length, uint32_t hash);
static uint32_t hashString(const char* string, int length);

StringKey* copyString(const char* chars, int length) {
	uint32_t hash = hashString(chars, length);

	char* heapChars = ALLOCATE(char, length + 1);
	memcpy(heapChars, chars, length);
	heapChars[length] = '\0';

	return allocateString(heapChars, length, hash);
}

StringKey* takeString(char* chars, int length) {
	uint32_t hash = hashString(chars, length);
	return allocateString(chars, length, hash);
}

void buildString(StringKey* string, const char* chars) {
	size_t length = strlen(chars);
	string->length = length;
	string->chars  = chars;
	string->hash   = hashString(chars, length);
}

static StringKey* allocateString(const char* chars, int length, uint32_t hash) {
	StringKey* string = ALLOCATE(StringKey, 1);
	string->length    = length;
	string->chars     = chars;
	string->hash      = hash;

	return string;
}

//uses the FNV-1a algorithm to produce the hash
static uint32_t hashString(const char* string, int length) {
	uint32_t hash = 2166136261u;
	for(int i = 0; i < length; ++ i) {
		hash ^= (uint8_t) string[i];
		hash *= 16777619;
	}

	return hash;
}