#include "hashmap/hashmap.h"
#include "memory/memory.h"

#include <stdlib.h>
#include <string.h>

#define HASHMAP_MAX_LOAD 0.75

void initHashMap(HashMap* hashmap) {
	hashmap->capacity = 0;
	hashmap->count    = 0;
	hashmap->entries  = NULL;
}

void freeHashMap(HashMap* hashmap) {
	FREE_ARRAY(hashmap->entries);
	initHashMap(hashmap);
}

static Entry* findEntry(Entry* entries, int capacity, StringKey* key) {
	uint32_t index = key->hash % capacity;
	Entry* tombstone = NULL;

	for(;;) {
		Entry* entry = &entries[index];
		if(entry->key == NULL) {
			if(IS_NULL(entry->value)) {
				return tombstone ? tombstone : entry;
			} else {
				if(tombstone == NULL) tombstone = entry;
			}
		} else if(entry->key == key) {
			return entry;
		}

		index = (index + 1) % capacity;
	}
}

static void adjustCapacity(HashMap* hashmap, int capacity) {
	Entry* entries = ALLOCATE(Entry, capacity);
	for(int i = 0; i < capacity; ++ i) {
		entries[i].key   = NULL;
		entries[i].value = NULL_VALUE;
	}

	hashmap->count = 0;
	for(int i = 0; i < hashmap->capacity; ++ i) {
		Entry* entry = &hashmap->entries[i];
		if(entry->key == NULL) continue;

		Entry* dest = findEntry(entries, capacity, entry->key);
		dest->key   = entry->key;
		dest->value = entry->value;

		hashmap->count ++;
	}

	FREE_ARRAY(hashmap->entries);

	hashmap->entries  = entries;
	hashmap->capacity = capacity;
}

bool insertHashMap(HashMap* hashmap, StringKey* key, Value value) {
	if(hashmap->count + 1 > hashmap->capacity * HASHMAP_MAX_LOAD) {
		int capacity = GROW_CAPACITY(hashmap->capacity);
		adjustCapacity(hashmap, capacity);
	}

	Entry* entry = findEntry(hashmap->entries, hashmap->capacity, key);
	bool isNew = entry->key == NULL;
	if(isNew) hashmap->count ++;

	entry->key   = key;
	entry->value = value;
	return isNew;
}

bool getHashMap(HashMap* hashmap, StringKey* key, Value* value) {
	if(hashmap->count == 0) return false;

	Entry* entry = findEntry(hashmap->entries, hashmap->capacity, key);
	if(entry->key == NULL) return false;

	*value = entry->value;
	return true;
}

bool deleteHashMap(HashMap* hashmap, StringKey* key) {
	if(hashmap->count == 0) return false;

	Entry* entry = findEntry(hashmap->entries, hashmap->capacity, key);
	if(entry->key == NULL) return false;

	entry->key   = NULL;
	entry->value = BOOL_VALUE(true);
	return true;
}

void copyHashMap(HashMap* from, HashMap* to) {
	for(int i = 0; i < from->capacity; ++ i) {
		Entry* entry = &from->entries[i];
		if(entry->key != NULL) {
			insertHashMap(to, entry->key, entry->value);
		}
	}
}

StringKey* findStringHashMap(HashMap* hashmap, const char* chars, int length, uint32_t hash) {
	if(hashmap->count == 0) return NULL;

	uint32_t index = hash % hashmap->capacity;
	for(;;) {
		Entry* entry = &hashmap->entries[index];
		if(entry->key == NULL) {
			if(IS_NULL(entry->value)) return NULL;
		} else if(entry->key->length == length
			   && entry->key->hash == hash
			   && !memcmp(entry->key->chars, chars, length)) {

			return entry->key;
		}

		index = (index + 1) % hashmap->capacity;
	}
}