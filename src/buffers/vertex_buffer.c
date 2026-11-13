#include "buffers/vertex_buffer.h"
#include "buffers/buffer_base.h"
#include "common/opengl_common.h"

#define VBO_BUFFER_PTR(vbo) (&((vbo)->buffer))

static void resetData(VBO* vbo);

void initVBO(VBO* vbo) {
	resetData(vbo);
	initBuffer(VBO_BUFFER_PTR(vbo));
}

void initVBO_FromData(VBO* vbo, const void* data, size_t size) {
	initVBO(vbo);
	setTargetVBO(vbo);
	sendDataVBO(vbo, data, size);
}

void freeVBO(VBO* vbo) {
	//also resets the buffer
	freeBuffer(VBO_BUFFER_PTR(vbo));
	resetData(vbo);
}

void sendDataVBO(VBO* vbo, const void* data, size_t size) {
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) size, data, GL_STATIC_DRAW);
	vbo->size = size;
}

void setTargetVBO(VBO* vbo) {
	setTargetBuffer(VBO_BUFFER_PTR(vbo), GL_ARRAY_BUFFER);
}

void resetTargetVBO() {
	resetTargetBuffer(GL_ARRAY_BUFFER);
}

static void resetData(VBO* vbo) {
	vbo->size = 0;
}