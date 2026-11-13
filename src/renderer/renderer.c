#include "renderer/renderer.h"

static void resetData(Renderer* renderer);

void initRenderer(Renderer* renderer) {
	resetData(renderer);
}

void freeRenderer(Renderer* renderer) {
	resetData(renderer);
}

void drawRenderer(Renderer* renderer) {
	setTargetVAO(renderer->vao);
	setTargetShader(renderer->shader);
	setTargetVBO(renderer->vbo);

	if(renderer->ibo) {
		setTargetIBO(renderer->ibo);
		glDrawElements(GL_TRIANGLES, renderer->ibo->count, renderer->ibo->type, NULL);
	} else {
		glDrawArrays(GL_TRIANGLES, 0, renderer->vbo->size / renderer->vao->totalAttributeCount);
	}
}

void setVAO_Renderer(Renderer* renderer, VAO* vao) {
	renderer->vao = vao;
}

void setVBO_Renderer(Renderer* renderer, VBO* vbo) {
	renderer->vbo = vbo;
}

void setIBO_Renderer(Renderer* renderer, IBO* ibo) {
	renderer->ibo = ibo;
}

void setShader_Renderer(Renderer* renderer, Shader* shader) {
	renderer->shader = shader;
}

void setMode_Renderer(Renderer* renderer, GLenum mode) {
	renderer->mode = mode;
}

void draw(VBO* vbo, VAO* vao, Shader* shader, GLenum mode) {
	setTargetVAO(vao);
	setTargetShader(shader);
	setTargetVBO(vbo);
	glDrawArrays(mode, 0, vbo->size / vao->totalAttributeCount);
}

void drawIndex(VBO* vbo, VAO* vao, IBO* ibo, Shader* shader, GLenum mode) {
	setTargetVAO(vao);
	setTargetShader(shader);
	setTargetVBO(vbo);
	setTargetIBO(ibo);
	glDrawElements(mode, ibo->count, ibo->type, NULL);
}

void clearColor(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
}

void clearBuffers(uint16_t mask) {
	glClear(mask);
}

static void resetData(Renderer* renderer) {
	renderer->vao    = NULL;
	renderer->shader = NULL;
	renderer->vbo    = NULL;
	renderer->ibo    = NULL;
	renderer->mode   = 0;
}