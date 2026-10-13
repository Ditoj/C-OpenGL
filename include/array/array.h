#ifndef opengl_array_h
#define opengl_array_h

//only works for non-void* arrays
#define ARRAY_SIZE(array)  sizeof(array)

//only works for non-void* arrays
#define ARRAY_COUNT(array) (sizeof(array) / sizeof(array[0]))

#endif