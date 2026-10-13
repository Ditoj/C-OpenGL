#include "buffers/buffer_base.h"
#include "common/opengl_common.h"

void initBuffer(Buffer* buffer) {
	glGenBuffers(1, &buffer->index);
}

static void resetData(Buffer* buffer);

void freeBuffer(Buffer* buffer) {
	glDeleteBuffers(1, &buffer->index);
	resetData(buffer);
}

void sendDataBuffer(const void* data, size_t size) {
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) size, data, GL_STATIC_DRAW);
}

void setTargetBuffer(Buffer* buffer, GLenum target) {
	glBindBuffer(target, buffer->index);
}

void resetTargetBuffer(GLenum target) {
	glBindBuffer(target, 0);
}

static void resetData(Buffer* buffer) {
	buffer->index = 0;
}