#include "buffers/vertex_buffer.h"
#include "buffers/buffer_base.h"
#include "common/opengl_common.h"

#define VBO_BUFFER_PTR(vbo) (&((vbo)->buffer))

void initVBO(VBO* vbo) {
	initBuffer(VBO_BUFFER_PTR(vbo));
}

void freeVBO(VBO* vbo) {
	//also resets the buffer
	freeBuffer(VBO_BUFFER_PTR(vbo));
}

void sendDataVBO(VBO* vbo, const void* data, size_t size) {
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) size, data, GL_STATIC_DRAW);
}

void setTargetVBO(VBO* vbo) {
	setTargetBuffer(VBO_BUFFER_PTR(vbo), GL_ARRAY_BUFFER);
}

void resetTargetVBO() {
	resetTargetBuffer(GL_ARRAY_BUFFER);
}