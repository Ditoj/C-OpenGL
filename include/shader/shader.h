#ifndef program_shader_h
#define program_shader_h

typedef enum {
	SHADER_FRAGMENT,
	SHADER_VERTEX,
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
int linkShaderProgram(ShaderProgram* program);
void setTargetProgram(ShaderProgram* program);
void resetTargetProgram();

void initShader(Shader* shader);
void freeShader(Shader* shader);
int loadShaderFromFile(Shader* shader, const char* path);
int loadShaderFromSource(Shader* shader, SHADER_TYPE type, char* source);

#endif