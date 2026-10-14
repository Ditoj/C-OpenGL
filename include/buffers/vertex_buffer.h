#ifndef opengl_vbo_h
#define opengl_vbo_h

#include "buffers/buffer_base.h"

typedef struct {
	Buffer buffer;
	size_t size;
} VBO;

void initVBO(VBO* vbo);
void initVBO_FromData(VBO* vbo, const void* data, size_t size);
void freeVBO(VBO* vbo);
void sendDataVBO(VBO* vbo, const void* data, size_t size);
void setTargetVBO(VBO* vbo);
void resetTargetVBO();

#endif