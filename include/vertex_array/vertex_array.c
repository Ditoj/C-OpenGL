#include "vertex_array/vertex_array.h"
#include "glad/glad.h"
#include "glfw/glfw3.h"

void initVAO(VAO* vao) {
	vao->index = 0;
}

void freeVAO(VAO* vao) {
	glDeleteVertexArrays(1, &vao->index);
	initVAO(vao);
}

void generateVAO(VAO* vao) {
	glGenVertexArrays(1, &vao->index);
}

void updateVAO(VAO* vao) {
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void *) 0);
}

void setTargetVAO(VAO* vao) {
	glBindVertexArray(vao->index);
}

void resetTargetVAO() {
	glBindVertexArray(0);
}