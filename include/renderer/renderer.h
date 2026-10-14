#ifndef opengl_renderer_h
#define opengl_renderer_h

#include "common/opengl_common.h"
#include "common/common.h"
#include "vertex_array/vertex_array.h"
#include "buffers/vertex_buffer.h"
#include "buffers/index_buffer.h"
#include "shader/shader.h"

typedef struct {
	VAO* vao;
	Shader* shader;
	VBO* vbo;
	IBO* ibo;
	GLenum mode;
} Renderer;

void initRenderer(Renderer* renderer);
void freeRenderer(Renderer* renderer);
void drawRenderer(Renderer* renderer);
void setVAO_Renderer(Renderer* renderer, VAO* vao);
void setVBO_Renderer(Renderer* renderer, VBO* vbo);
void setIBO_Renderer(Renderer* renderer, IBO* ibo);
void setShader_Renderer(Renderer* renderer, Shader* shader);
void setMode_Renderer(Renderer* renderer, GLenum mode);
void draw(VBO* vbo, VAO* vao, Shader* shader, GLenum mode);
void drawIndex(VBO* vbo, VAO* vao, IBO* ibo, Shader* shader, GLenum mode);
void clearColor(float r, float g, float b, float a);
void clearBuffers(uint16_t mask);

#endif