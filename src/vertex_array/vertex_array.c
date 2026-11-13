#include "vertex_array/vertex_array.h"

static void resetData(VAO* vao);

void initVAO(VAO* vao) {
	resetData(vao);
	glGenVertexArrays(1, &vao->index);
}

void freeVAO(VAO* vao) {
	glDeleteVertexArrays(1, &vao->index);
	resetData(vao);
}

void addAttributeVAO(VAO* vao, int count, GLenum type) {
	if(vao->attributeCount >= VAO_ATTRIBUTE_MAX || count < 1 || count > 4) return;

	size_t typeSize = 0;
	switch(type) {
		case GL_FLOAT:          typeSize = sizeof(GLfloat);  break;
		case GL_INT:            typeSize = sizeof(GLint);    break;
		case GL_UNSIGNED_INT:   typeSize = sizeof(GLuint);   break;
		case GL_SHORT:          typeSize = sizeof(GLshort);  break;
		case GL_UNSIGNED_SHORT: typeSize = sizeof(GLushort); break;
		case GL_BYTE:           typeSize = sizeof(GLbyte);   break;
		case GL_UNSIGNED_BYTE:  typeSize = sizeof(GLubyte);  break;

		default: return; //unreachable
	}

	size_t typedSize = typeSize * count;

	vao->layout[vao->attributeCount ++] = (VertexAttributeLayout) {
		.size  = typedSize,
		.count = count,
		.type  = type
	};

	vao->stride              += typedSize;
	vao->totalAttributeCount += count;
}

void patchVAO(VAO* vao) {
	size_t offset = 0;
	for(GLuint i = 0; i < vao->attributeCount; ++ i) {
		VertexAttributeLayout* attr = &vao->layout[i];

		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, attr->count, attr->type, GL_FALSE, (GLsizei) vao->stride, (const void *) offset);

		offset += attr->size;
	}
}

void setTargetVAO(VAO* vao) {
	glBindVertexArray(vao->index);
}

void resetTargetVAO() {
	glBindVertexArray(0);
}

static void resetData(VAO* vao) {
	vao->offset              = 0;
	vao->stride              = 0;
	vao->index               = 0;
	vao->attributeCount      = 0;
	vao->totalAttributeCount = 0;
}