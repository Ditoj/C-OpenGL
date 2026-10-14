#ifndef opengl_ibo_h
#define opengl_ibo_h

#include "buffers/buffer_base.h"

#define IBO_AS_BUFFER(ibo) (Buffer *) (ibo)

typedef struct {
	Buffer buffer;
	size_t count;
	GLenum type;
} IBO;

void initIBO(IBO* ibo);
void initIBO_FromData(IBO* ibo, const void* data, size_t size, GLenum type);
void freeIBO(IBO* ibo);
void sendDataIBO(IBO* ibo, const void* data, size_t size, GLenum type);
void setTargetIBO(IBO* ibo);
void resetTargetIBO();

#endif