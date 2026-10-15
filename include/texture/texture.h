#ifndef opengl_texture_h
#define opengl_texture_h

typedef struct {
	int width, height, bpp;
	unsigned int id;
} Texture2D;

void initTexture2D(Texture2D* texture);
int initTexture2D_fromImage(Texture2D* texture, const char* path, int slot);
void freeTexture2D(Texture2D* texture);
void setTextureParameters2D();
int loadImageTexture2D(Texture2D* texture, const char* path);
void setTargetTexture2D(Texture2D* texture, int slot);
void resetTargetTexture2D();

#endif