#ifndef program_shader_h
#define program_shader_h

#include "common/common.h"

typedef enum {
	SHADER_VERTEX,
	SHADER_FRAGMENT,
	SHADER_GEOMETRY,
	SHADER_COMPUTE,
	SHADER_ERROR
} SHADER_TYPE;

typedef struct {
	SHADER_TYPE type;
	char* source; //owns the string
	unsigned int index;
} Shader;

typedef struct {
	unsigned int index;
	Shader* attachedShaders[4];
} ShaderProgram;

void initShaderProgram(ShaderProgram* program);
void freeShaderProgram(ShaderProgram* program);
void freeAttachedShaders(ShaderProgram* program);
void attachShaderProgram(ShaderProgram* program, Shader* shader);
int linkShaderProgram(ShaderProgram* program, bool deleteShaders);
void setTargetProgram(ShaderProgram* program);
void resetTargetProgram();

void initShader(Shader* shader);
void freeShader(Shader* shader);
int loadShaderFromFile(Shader* shader, SHADER_TYPE type, const char* path);
int loadShaderFromSource(Shader* shader, SHADER_TYPE type, char* source);

#endif