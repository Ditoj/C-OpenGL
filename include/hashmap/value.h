#ifndef opengl_value_h
#define opengl_value_h

#include "common/common.h"

#define IS_BOOL(value) ((value).type == VALUE_BOOL)
#define IS_NUM(value)  ((value).type == VALUE_NUM)
#define IS_NULL(value) ((value).type == VALUE_NULL)

#define AS_BOOL(value) ((value).as.boolean)
#define AS_NUM(value)  ((value).as.number)

#define BOOL_VALUE(value) ((Value) {VALUE_BOOL, {.boolean = value}})
#define NUM_VALUE(value)  ((Value) {VALUE_NUM,  {.number = value }})
#define NULL_VALUE        ((Value) {VALUE_NULL, {.number = 0     }})

typedef enum {
	VALUE_NUM,
	VALUE_BOOL,
	VALUE_NULL
} VALUE_TYPE;

typedef struct {
	VALUE_TYPE type;

	union {
		bool boolean;
		double number;
	} as;
} Value;

#endif