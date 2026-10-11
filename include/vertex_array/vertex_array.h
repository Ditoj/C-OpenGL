#ifndef project_vao_h
#define project_vao_h

#include "common/common.h"
#include "glad/glad.h"
#include "glfw/glfw3.h"

#define VAO_ATTRIBUTE_MAX 16

typedef struct {
	size_t size;
	int count;
	GLenum type;
} VertexAttributeLayout;

typedef struct {
	VertexAttributeLayout layout[VAO_ATTRIBUTE_MAX];
	size_t offset;
	size_t stride;
	unsigned int index;
	unsigned int attributeCount;
} VAO;

void initVAO(VAO* vao);
void freeVAO(VAO* vao);
void addAttributeVAO(VAO* vao, int count, GLenum type);
void patchVAO(VAO* vao);
void setTargetVAO(VAO* vao);
void resetTargetVAO();

#endif