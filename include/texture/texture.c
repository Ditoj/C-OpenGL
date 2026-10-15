#include "texture/texture.h"
#include "common/opengl_common.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

#include <stdio.h>

static void resetData(Texture2D* texture);

void initTexture2D(Texture2D* texture) {
	resetData(texture);
	glGenTextures(1, &texture->id);
}

int initTexture2D_fromImage(Texture2D* texture, const char* path, int slot) {
	initTexture2D(texture);
	setTextureParameters2D();
	setTargetTexture2D(texture, 0);

	return loadImageTexture2D(texture, "../img/container.png");
}

void freeTexture2D(Texture2D* texture) {
	glDeleteTextures(1, &texture->id);
	resetData(texture);
}

void setTextureParameters2D() {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
}

int loadImageTexture2D(Texture2D* texture, const char* path) {
	unsigned char* data = stbi_load(path, &texture->width, &texture->height, &texture->bpp, 0);

	uint16_t format = texture->bpp == 4 ? GL_RGBA : GL_RGB;

	if(data) {
		glTexImage2D(GL_TEXTURE_2D, 0, format, texture->width, texture->height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);

		return 1;
	}

	fprintf(stderr, "Error loading texture at \"%s\", reason: %s", path, stbi_failure_reason());
	return 0;
}

void setTargetTexture2D(Texture2D* texture, int slot) {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texture->id);
}

void resetTargetTexture2D() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

static void resetData(Texture2D* texture) {
	texture->width  = 0;
	texture->height = 0;
	texture->bpp    = 0;
	texture->id     = 0;
}