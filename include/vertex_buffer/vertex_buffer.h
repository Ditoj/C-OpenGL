#ifndef project_vbo_h
#define project_vbo_h

#include "common/common.h"

typedef struct {
	unsigned int index;
	const void* data;
} VBO;

void initVBO(VBO* vbo);
void freeVBO(VBO* vbo);
void sendDataVBO(const void* data, size_t size);
void setTargetVBO(VBO* vbo);
void resetTargetVBO();

#endif