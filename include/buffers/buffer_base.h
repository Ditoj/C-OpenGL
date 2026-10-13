#ifndef opengl_buffer_h
#define opengl_buffer_h

#include "common/opengl_common.h"

typedef struct {
	unsigned int index;
} Buffer;

void initBuffer(Buffer* buffer);
void freeBuffer(Buffer* buffer);
void sendDataBuffer(const void* data, size_t size);
void setTargetBuffer(Buffer* buffer, GLenum target);
void resetTargetBuffer(GLenum target);

#endif