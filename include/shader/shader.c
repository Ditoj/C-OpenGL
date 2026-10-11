#include "shader/shader.h"
#include "file/file.h"
#include "glad/glad.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initShaderProgram(ShaderProgram* program) {
	if(program->index != 0 && glIsProgram(program->index)) glDeleteProgram(program->index);

	program->index = glCreateProgram();
	for(int i = 0; i < 4; ++ i) program->attachedShaders[i] = NULL;
}

void freeShaderProgram(ShaderProgram* program) {
	glDeleteProgram(program->index);
	initShaderProgram(program);
}

void freeAttachedShaders(ShaderProgram* program) {
	if(program->attachedShaders[SHADER_FRAGMENT]) freeShader(program->attachedShaders[SHADER_FRAGMENT]);
	if(program->attachedShaders[SHADER_VERTEX])   freeShader(program->attachedShaders[SHADER_VERTEX]);
	if(program->attachedShaders[SHADER_GEOMETRY]) freeShader(program->attachedShaders[SHADER_GEOMETRY]);
	if(program->attachedShaders[SHADER_COMPUTE])  freeShader(program->attachedShaders[SHADER_COMPUTE]);
}

void attachShaderProgram(ShaderProgram* program, Shader* shader) {
	int mask = 0;
	switch(shader->type) {
		case SHADER_FRAGMENT: {
			if(!program->attachedShaders[SHADER_FRAGMENT]) {
				program->attachedShaders[SHADER_FRAGMENT] = shader;
				break;
			}

			return; //shader already attached
		}

		case SHADER_VERTEX: {
			if(!program->attachedShaders[SHADER_VERTEX]) {
				program->attachedShaders[SHADER_VERTEX] = shader;
				break;
			}

			return; //shader already attached
		}

		case SHADER_GEOMETRY: {
			if(!program->attachedShaders[SHADER_GEOMETRY]) {
				program->attachedShaders[SHADER_GEOMETRY] = shader;
				break;
			}

			return; //shader already attached
		}

		case SHADER_COMPUTE: {
			if(!program->attachedShaders[SHADER_COMPUTE]) {
				program->attachedShaders[SHADER_COMPUTE] = shader;
				break;
			}

			return; //shader already attached
		}
	}

	glAttachShader(program->index, shader->index);
}

int linkShaderProgram(ShaderProgram* program) {
	glLinkProgram(program->index);

	GLint success;
	glGetProgramiv(program->index, GL_LINK_STATUS, &success);
	if(!success) {
		char errorLog[1024];
		glGetProgramInfoLog(program->index, 1024, NULL, errorLog);

		fprintf(stderr, errorLog);

		return 0;
	}

	if(program->attachedShaders[SHADER_FRAGMENT]) glDetachShader(program->index, program->attachedShaders[SHADER_FRAGMENT]->index);
	if(program->attachedShaders[SHADER_VERTEX])   glDetachShader(program->index, program->attachedShaders[SHADER_VERTEX]->index);
	if(program->attachedShaders[SHADER_GEOMETRY]) glDetachShader(program->index, program->attachedShaders[SHADER_GEOMETRY]->index);
	if(program->attachedShaders[SHADER_COMPUTE])  glDetachShader(program->index, program->attachedShaders[SHADER_COMPUTE]->index);

	return 1;
}

void setTargetProgram(ShaderProgram* program) {
	glUseProgram(program->index);
}

void resetTargetProgram() {
	glUseProgram(0);
}

void initShader(Shader* shader) {
	shader->type   = SHADER_ERROR;
	shader->source = NULL;
}

void freeShader(Shader* shader) {
	free(shader->source);
	glDeleteShader(shader->index);

	initShader(shader);
}

static int loadShader(Shader* shader);

int loadShaderFromFile(Shader* shader, const char* path) {
	char type[256];
	getFileExtension(path, type);

	if(!strncmp(type, "fsh", 3)) shader->type = SHADER_FRAGMENT;
	if(!strncmp(type, "vsh", 3)) shader->type = SHADER_VERTEX;
	if(!strncmp(type, "gsh", 3)) shader->type = SHADER_GEOMETRY;
	if(!strncmp(type, "csh", 3)) shader->type = SHADER_COMPUTE;

	shader->source = readFile(path);

	return loadShader(shader);
}

int loadShaderFromSource(Shader* shader, SHADER_TYPE type, char* source) {
	shader->type   = type;
	shader->source = source;

	return loadShader(shader);
}

static int loadShader(Shader* shader) {
	if(shader->type == SHADER_ERROR) return 0;

	GLenum type;
	switch(shader->type) {
		case SHADER_FRAGMENT: type = GL_FRAGMENT_SHADER; break;
		case SHADER_VERTEX:   type = GL_VERTEX_SHADER;   break;
		case SHADER_GEOMETRY: type = GL_GEOMETRY_SHADER; break;
		case SHADER_COMPUTE:  type = GL_COMPUTE_SHADER;  break;
	}

	shader->index = glCreateShader(type);
	const GLchar* source = shader->source;
	glShaderSource(shader->index, 1, &source, NULL);
	glCompileShader(shader->index);

	GLint success;
	glGetShaderiv(shader->index, GL_COMPILE_STATUS, &success);
	if(!success) {
		char errorLog[1024];
		glGetShaderInfoLog(shader->index, 1024, NULL, errorLog);

		fprintf(stderr, errorLog);

		return 0;
	}

	return 1;
}