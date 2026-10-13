#include "shader/shader.h"
#include "common/opengl_common.h"
#include "file/file.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initShaderProgram(ShaderProgram* program) {
	program->index = glCreateProgram();
	for(int i = 0; i < 4; ++ i) program->attachedShaders[i] = NULL;
}

static void resetData(ShaderProgram* program);

void freeShaderProgram(ShaderProgram* program) {
	glDeleteProgram(program->index);
	resetData(program);
}

void freeAttachedShaders(ShaderProgram* program) {
	if(program->attachedShaders[SHADER_VERTEX])   freeShader(program->attachedShaders[SHADER_VERTEX]);
	if(program->attachedShaders[SHADER_FRAGMENT]) freeShader(program->attachedShaders[SHADER_FRAGMENT]);
	if(program->attachedShaders[SHADER_GEOMETRY]) freeShader(program->attachedShaders[SHADER_GEOMETRY]);
	if(program->attachedShaders[SHADER_COMPUTE])  freeShader(program->attachedShaders[SHADER_COMPUTE]);
}

void attachShaderProgram(ShaderProgram* program, Shader* shader) {
	int mask = 0;
	switch(shader->type) {
		case SHADER_VERTEX: {
			if(!program->attachedShaders[SHADER_VERTEX]) {
				program->attachedShaders[SHADER_VERTEX] = shader;
				break;
			}

			return; //shader already attached
		}

		case SHADER_FRAGMENT: {
			if(!program->attachedShaders[SHADER_FRAGMENT]) {
				program->attachedShaders[SHADER_FRAGMENT] = shader;
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

int linkShaderProgram(ShaderProgram* program, bool deleteShaders) {
	glLinkProgram(program->index);

	GLint success;
	glGetProgramiv(program->index, GL_LINK_STATUS, &success);
	if(!success) {
		char errorLog[1024];
		glGetProgramInfoLog(program->index, 1024, NULL, errorLog);

		fprintf(stderr, "%s", errorLog);
		return 0;
	}

	if(deleteShaders) {
		for(int i = 0; i < 4; ++ i) {
			if(program->attachedShaders[i]) {
				glDetachShader(program->index, program->attachedShaders[i]->index);
				freeShader(program->attachedShaders[i]);
				program->attachedShaders[i] = NULL;
			}
		}
	}

	return 1;
}

void setTargetProgram(ShaderProgram* program) {
	glUseProgram(program->index);
}

void resetTargetProgram() {
	glUseProgram(0);
}

void initShader(Shader* shader) {
	shader->index  = 0;
	shader->type   = SHADER_ERROR;
	shader->source = NULL;
}

void freeShader(Shader* shader) {
	free(shader->source);
	glDeleteShader(shader->index);

	initShader(shader);
}

static int loadShader(Shader* shader);

int loadShaderFromFile(Shader* shader, SHADER_TYPE type, const char* path) {
	/*char type[256];
	getFileExtension(path, type);

	if(!strncmp(type, "vsh", 3)) shader->type = SHADER_VERTEX;
	if(!strncmp(type, "fsh", 3)) shader->type = SHADER_FRAGMENT;
	if(!strncmp(type, "gsh", 3)) shader->type = SHADER_GEOMETRY;
	if(!strncmp(type, "csh", 3)) shader->type = SHADER_COMPUTE;*/

	shader->source = readFile(path);
	shader->type   = type;

	return loadShader(shader);
}

int loadShaderFromSource(Shader* shader, SHADER_TYPE type, char* source) {
	shader->type   = type;
	shader->source = strdup(source);

	return loadShader(shader);
}

static int loadShader(Shader* shader) {
	if(shader->type == SHADER_ERROR) return 0;

	GLenum type;
	switch(shader->type) {
		case SHADER_VERTEX:   type = GL_VERTEX_SHADER;   break;
		case SHADER_FRAGMENT: type = GL_FRAGMENT_SHADER; break;
		case SHADER_GEOMETRY: type = GL_GEOMETRY_SHADER; break;
		case SHADER_COMPUTE:  type = GL_COMPUTE_SHADER;  break;

		default: return 0;
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

		fprintf(stderr, "%s", errorLog);
		return 0;
	}

	return 1;
}

static void resetData(ShaderProgram* program) {
	program->index = 0;
	for(int i = 0; i < 4; ++ i) program->attachedShaders[i] = NULL;
}