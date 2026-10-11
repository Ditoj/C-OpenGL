#ifndef project_vao_h
#define project_vao_h

typedef struct {
	unsigned int index;
} VAO;

void initVAO(VAO* vao);
void freeVAO(VAO* vao);
void generateVAO(VAO* vao);
void updateVAO(VAO* vao);
void setTargetVAO(VAO* vao);
void resetTargetVAO();

#endif