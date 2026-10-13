#include "buffers/index_buffer.h"
#include "buffers/buffer_base.h"

#define IBO_BUFFER_PTR(ibo) (&((ibo)->buffer))

static void resetData(IBO* ibo);

void initIBO(IBO* ibo) {
	resetData(ibo);
	initBuffer(IBO_BUFFER_PTR(ibo));
}

void freeIBO(IBO* ibo) {
	freeBuffer(IBO_BUFFER_PTR(ibo));
	resetData(ibo);
}

void sendDataIBO(IBO* ibo, const void* data, size_t size, GLenum type) {
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr) size, data, GL_STATIC_DRAW);

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

	ibo->count = size / typeSize;
	ibo->type  = type;
}

void setTargetIBO(IBO* ibo) {
	setTargetBuffer(IBO_BUFFER_PTR(ibo), GL_ELEMENT_ARRAY_BUFFER);
}

void resetTargetIBO() {
	resetTargetBuffer(GL_ELEMENT_ARRAY_BUFFER);
}

static void resetData(IBO* ibo) {
	//the buffer is reset with freeBuffer(...)
	ibo->count  = 0;
	ibo->type   = 0;
}