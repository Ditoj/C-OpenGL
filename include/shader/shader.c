#include "shader/shader.h"
#include "common/opengl_common.h"
#include "file/file.h"
#include "memory/memory.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void resetData(Shader* shader);
static int getUniformLocation(Shader* shader, StringKey* name);

void initShader(Shader* shader) {
	for(int i = 0; i < 4; ++ i) shader->attachedComponents[i] = NULL;
	shader->uniformCache = ALLOCATE(HashMap, 1);
	initHashMap(shader->uniformCache);
	shader->index = glCreateProgram();
}

void freeShader(Shader* shader) {
	freeHashMap(shader->uniformCache);
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

		default: return; //unreachable
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

void uniformFromString(Uniform* uniform, const char* string) {
	buildString(uniform, string);
}

void setUniform1i(Shader* shader, Uniform* uniform, int value) {
	glUniform1i(getUniformLocation(shader, uniform), value);
}

void setUniform2i(Shader* shader, Uniform* uniform, int value1, int value2) {
	glUniform2i(getUniformLocation(shader, uniform), value1, value2);
}

void setUniform3i(Shader* shader, Uniform* uniform, int value1, int value2, int value3) {
	glUniform3i(getUniformLocation(shader, uniform), value1, value2, value3);
}

void setUniform4i(Shader* shader, Uniform* uniform, int value1, int value2, int value3, int value4) {
	glUniform4i(getUniformLocation(shader, uniform), value1, value2, value3, value4);
}

void setUniform1f(Shader* shader, Uniform* uniform, float value) {
	glUniform1f(getUniformLocation(shader, uniform), value);
}

void setUniform2f(Shader* shader, Uniform* uniform, float value1, float value2) {
	glUniform2f(getUniformLocation(shader, uniform), value1, value2);
}

void setUniform3f(Shader* shader, Uniform* uniform, float value1, float value2, float value3) {
	glUniform3f(getUniformLocation(shader, uniform), value1, value2, value3);
}

void setUniform4f(Shader* shader, Uniform* uniform, float value1, float value2, float value3, float value4) {
	glUniform4f(getUniformLocation(shader, uniform), value1, value2, value3, value4);
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
	for(int i = 0; i < 4; ++ i) shader->attachedComponents[i] = NULL;

	shader->uniformCache = NULL;
	shader->index        = 0;
}

static int getUniformLocation(Shader* shader, StringKey* name) {
	Value result;

	if(getHashMap(shader->uniformCache, name, &result)) {
		return AS_NUM(result);
	} else {
		int location = glGetUniformLocation(shader->index, name->chars);
		insertHashMap(shader->uniformCache, name, NUM_VALUE(location));

		return location;
	}
}