#ifndef opengl_hashmap_h
#define opengl_hashmap_h

#include "hashmap/value.h"
#include "hashmap/string_key.h"

typedef struct {
    StringKey* key;
    Value value;
} Entry;

typedef struct {
    int count;
    int capacity;
    Entry* entries;
} HashMap;

void initHashMap(HashMap* hashmap);
void freeHashMap(HashMap* hashmap);
bool insertHashMap(HashMap* hashmap, StringKey* key, Value value);
bool getHashMap(HashMap* hashmap, StringKey* key, Value* value);
bool deleteHashMap(HashMap* hashmap, StringKey* key);
void copyHashMap(HashMap* from, HashMap* to);
StringKey* findStringHashMap(HashMap* hashmap, const char* chars, int length, uint32_t hash);

#endif