#ifndef program_shader_h
#define program_shader_h

#include "common/common.h"
#include "hashmap/hashmap.h"
#include "hashmap/string_key.h"
#include "hashmap/value.h"
#include "cglm/cglm.h"

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
} ShdComp; //Shader Component

typedef struct {
	ShdComp* attachedComponents[4];
	HashMap* uniformCache;
	unsigned int index;
} Shader;

//using StringKey as a type name here is obnoxious
typedef StringKey Uniform;

void initShader(Shader* shader);
void freeShader(Shader* shader);
void freeComponents(Shader* shader);
void attachComponent(Shader* shader, ShdComp* component);
int linkShader(Shader* shader, bool deleteComponents);
void setTargetShader(Shader* shader);
void resetTargetShader();
void uniformFromString(Uniform* uniform, const char* string);
void setUniform1i(Shader* shader, Uniform* uniform, int value);
void setUniform2i(Shader* shader, Uniform* uniform, int value1, int value2);
void setUniform3i(Shader* shader, Uniform* uniform, int value1, int value2, int value3);
void setUniform4i(Shader* shader, Uniform* uniform, int value1, int value2, int value3, int value4);
void setUniform1f(Shader* shader, Uniform* uniform, float value);
void setUniform2f(Shader* shader, Uniform* uniform, float value1, float value2);
void setUniform3f(Shader* shader, Uniform* uniform, float value1, float value2, float value3);
void setUniform4f(Shader* shader, Uniform* uniform, float value1, float value2, float value3, float value4);
void setUniformMat3f(Shader* shader, Uniform* uniform, const mat3 matrix);
void setUniformMat4f(Shader* shader, Uniform* uniform, const mat4 matrix);

void initShdComp(ShdComp* component);
void freeShdComp(ShdComp* component);
int loadShdCompFromFile(ShdComp* component, SHADER_TYPE type, const char* path);
int loadShdCompFromSource(ShdComp* component, SHADER_TYPE type, char* source);

#endif