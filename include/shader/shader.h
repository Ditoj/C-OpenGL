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
} ShdComp; //Shader Component

typedef struct {
	unsigned int index;
	ShdComp* attachedComponents[4];
} Shader;

void initShader(Shader* shader);
void freeShader(Shader* shader);
void freeComponents(Shader* shader);
void attachComponent(Shader* shader, ShdComp* component);
int linkShader(Shader* shader, bool deleteComponents);
void setTargetShader(Shader* shader);
void resetTargetShader();

void initShdComp(ShdComp* component);
void freeShdComp(ShdComp* component);
int loadShdCompFromFile(ShdComp* component, SHADER_TYPE type, const char* path);
int loadShdCompFromSource(ShdComp* component, SHADER_TYPE type, char* source);

#endif