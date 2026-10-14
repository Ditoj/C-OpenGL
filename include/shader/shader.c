#include "shader/shader.h"
#include "common/opengl_common.h"
#include "file/file.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initShader(Shader* shader) {
	shader->index = glCreateProgram();
	for(int i = 0; i < 4; ++ i) shader->attachedComponents[i] = NULL;
}

static void resetData(Shader* shader);

void freeShader(Shader* shader) {
	glDeleteProgram(shader->index);
	resetData(shader);
}

void freeComponents(Shader* shader) {
	if(shader->attachedComponents[SHADER_VERTEX])   freeShdComp(shader->attachedComponents[SHADER_VERTEX]);
	if(shader->attachedComponents[SHADER_FRAGMENT]) freeShdComp(shader->attachedComponents[SHADER_FRAGMENT]);
	if(shader->attachedComponents[SHADER_GEOMETRY]) freeShdComp(shader->attachedComponents[SHADER_GEOMETRY]);
	if(shader->attachedComponents[SHADER_COMPUTE])  freeShdComp(shader->attachedComponents[SHADER_COMPUTE]);
}

void attachComponent(Shader* shader, ShdComp* component) {
	int mask = 0;
	switch(component->type) {
		case SHADER_VERTEX: {
			if(!shader->attachedComponents[SHADER_VERTEX]) {
				shader->attachedComponents[SHADER_VERTEX] = component;
				break;
			}

			return; //shader already attached
		}

		case SHADER_FRAGMENT: {
			if(!shader->attachedComponents[SHADER_FRAGMENT]) {
				shader->attachedComponents[SHADER_FRAGMENT] = component;
				break;
			}

			return; //shader already attached
		}

		case SHADER_GEOMETRY: {
			if(!shader->attachedComponents[SHADER_GEOMETRY]) {
				shader->attachedComponents[SHADER_GEOMETRY] = component;
				break;
			}

			return; //shader already attached
		}

		case SHADER_COMPUTE: {
			if(!shader->attachedComponents[SHADER_COMPUTE]) {
				shader->attachedComponents[SHADER_COMPUTE] = component;
				break;
			}

			return; //shader already attached
		}
	}

	glAttachShader(shader->index, component->index);
}

int linkShader(Shader* shader, bool deleteShaders) {
	glLinkProgram(shader->index);

	GLint success;
	glGetProgramiv(shader->index, GL_LINK_STATUS, &success);
	if(!success) {
		char errorLog[1024];
		glGetProgramInfoLog(shader->index, 1024, NULL, errorLog);

		fprintf(stderr, "%s", errorLog);
		return 0;
	}

	if(deleteShaders) {
		for(int i = 0; i < 4; ++ i) {
			if(shader->attachedComponents[i]) {
				glDetachShader(shader->index, shader->attachedComponents[i]->index);
				freeShdComp(shader->attachedComponents[i]);
				shader->attachedComponents[i] = NULL;
			}
		}
	}

	return 1;
}

void setTargetShader(Shader* shader) {
	glUseProgram(shader->index);
}

void resetTargetShader() {
	glUseProgram(0);
}

void initShdComp(ShdComp* component) {
	component->index  = 0;
	component->type   = SHADER_ERROR;
	component->source = NULL;
}

void freeShdComp(ShdComp* component) {
	free(component->source);
	glDeleteShader(component->index);

	initShdComp(component);
}

static int loadShdComp(ShdComp* component);

int loadShdCompFromFile(ShdComp* component, SHADER_TYPE type, const char* path) {
	/*char type[256];
	getFileExtension(path, type);

	if(!strncmp(type, "vsh", 3)) shader->type = SHADER_VERTEX;
	if(!strncmp(type, "fsh", 3)) shader->type = SHADER_FRAGMENT;
	if(!strncmp(type, "gsh", 3)) shader->type = SHADER_GEOMETRY;
	if(!strncmp(type, "csh", 3)) shader->type = SHADER_COMPUTE;*/

	component->source = readFile(path);
	component->type   = type;

	return loadShdComp(component);
}

int loadShdCompFromSource(ShdComp* component, SHADER_TYPE type, char* source) {
	component->type   = type;
	component->source = strdup(source);

	return loadShdComp(component);
}

static int loadShdComp(ShdComp* component) {
	if(component->type == SHADER_ERROR) return 0;

	GLenum type;
	switch(component->type) {
		case SHADER_VERTEX:   type = GL_VERTEX_SHADER;   break;
		case SHADER_FRAGMENT: type = GL_FRAGMENT_SHADER; break;
		case SHADER_GEOMETRY: type = GL_GEOMETRY_SHADER; break;
		case SHADER_COMPUTE:  type = GL_COMPUTE_SHADER;  break;

		default: return 0;
	}

	component->index = glCreateShader(type);
	const GLchar* source = component->source;
	glShaderSource(component->index, 1, &source, NULL);
	glCompileShader(component->index);

	GLint success;
	glGetShaderiv(component->index, GL_COMPILE_STATUS, &success);
	if(!success) {
		char errorLog[1024];
		glGetShaderInfoLog(component->index, 1024, NULL, errorLog);

		fprintf(stderr, "%s", errorLog);
		return 0;
	}

	return 1;
}

static void resetData(Shader* shader) {
	shader->index = 0;
	for(int i = 0; i < 4; ++ i) shader->attachedComponents[i] = NULL;
}