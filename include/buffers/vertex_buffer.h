#ifndef opengl_vbo_h
#define opengl_vbo_h

#include "buffers/buffer_base.h"

typedef struct {
	Buffer buffer;
} VBO;

void initVBO(VBO* vbo);
void freeVBO(VBO* vbo);
void sendDataVBO(VBO* vbo, const void* data, size_t size);
void setTargetVBO(VBO* vbo);
void resetTargetVBO();

#endif