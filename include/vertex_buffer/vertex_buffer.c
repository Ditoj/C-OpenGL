#include "vertex_buffer/vertex_buffer.h"
#include "glad/glad.h"
#include "glfw/glfw3.h"

void initVBO(VBO* vbo) {
	vbo->index = 0;
	vbo->data  = NULL;
}

void freeVBO(VBO* vbo) {
	glDeleteBuffers(1, &vbo->index);
	initVBO(vbo);
}

void generateVBO(VBO* vbo) {
	glGenBuffers(1, &vbo->index);
}

void updateVBO(VBO* vbo, const void* data, size_t size) {
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr) size, data, GL_STATIC_DRAW);
}

void setTargetVBO(VBO* vbo) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo->index);
}

void resetTargetVBO() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}