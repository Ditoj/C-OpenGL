#ifndef opengl_key_h
#define opengl_key_h

#include "common/common.h"

typedef struct {
	size_t length;
	char* chars;
	uint32_t hash;
} StringKey;

//makes a copy of the given string and owns it
StringKey* copyString(const char* chars, int length);

//takes ownership of the given string
StringKey* takeString(char* chars, int length);

/*this is a loaded firearm that will be triggered if not used with string literals, as
it does not make the StringKey take ownership of the given string. USE THIS CAREFULLY*/
void buildString(StringKey* string, const char* chars);

#endif