#include "common/common.h"
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "array/array.h"
#include "buffers/vertex_buffer.h"
#include "buffers/index_buffer.h"
#include "vertex_array/vertex_array.h"
#include "shader/shader.h"
#include "renderer/renderer.h"
#include "texture/texture.h"

#include <stdio.h>
#include <stdlib.h>

#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480
#define WINDOW_TITLE  "DitoJ"

#define EXIT_GLFW_LOAD_ERROR 1
#define EXIT_GLAD_LOAD_ERROR 2

#define PROJECT_ROOT_PATH "../../"

static const float vertexData[] = {
	//x    y  r   g   b   u   v
	-.5, -.5, 1., 0., 0., 0., 0., //bottom-left
	 .5, -.5, 0., 1., 0., 1., 0., //bottom-right
	 .5,  .5, 0., 0., 1., 1., 1., //top-right
	-.5,  .5, 1., 1., 1., 0., 1.  //top-left
};

static unsigned char indexData[] = {
	0, 1, 2,
	2, 3, 0
};

static void errorCallback(int error, const char* description);
static void error(int code, const char* description);

void initGLFW_GLAD(GLFWwindow** window);

int main() {
	GLFWwindow* window;
	initGLFW_GLAD(&window);

	VBO vbo;
	initVBO_FromData(&vbo, vertexData, sizeof(vertexData));

	VAO vao;
	initVAO(&vao);
	setTargetVAO(&vao);
	addAttributeVAO(&vao, 2, GL_FLOAT);
	addAttributeVAO(&vao, 3, GL_FLOAT);
	addAttributeVAO(&vao, 2, GL_FLOAT);
	patchVAO(&vao);

	IBO ibo;
	initIBO_FromData(&ibo, indexData, sizeof(indexData), GL_UNSIGNED_BYTE);

	Shader shader;
	initShader(&shader);

	ShdComp fragment;
	ShdComp vertex;
	initShdComp(&fragment);
	initShdComp(&vertex);
	loadShdCompFromFile(&vertex, SHADER_VERTEX, PROJECT_ROOT_PATH "shader/base/base.vsh");
	loadShdCompFromFile(&fragment, SHADER_FRAGMENT, PROJECT_ROOT_PATH "shader/base/base.fsh");

	attachComponent(&shader, &fragment);
	attachComponent(&shader, &vertex);

	linkShader(&shader, true);

	glClearColor(0.1f, 0.2f, 0.6f, 1.f);

	Renderer renderer;
	initRenderer(&renderer);
	setVAO_Renderer(&renderer, &vao);
	setVBO_Renderer(&renderer, &vbo);
	setIBO_Renderer(&renderer, &ibo);
	setShader_Renderer(&renderer, &shader);
	setMode_Renderer(&renderer, GL_TRIANGLES);

	Texture2D texture;
	initTexture2D_fromImage(&texture, PROJECT_ROOT_PATH "img/container.png", 0);

	setTargetShader(&shader);
	setUniform1i(&shader, "u_texture", 0);

	while(!glfwWindowShouldClose(window)) {
		clearBuffers(GL_COLOR_BUFFER_BIT);
		drawRenderer(&renderer);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	freeVBO(&vbo);
	freeVAO(&vao);
	freeIBO(&ibo);
	freeShader(&shader);
	freeRenderer(&renderer);
	freeTexture2D(&texture);

	glfwDestroyWindow(window);
	glfwTerminate();
}

static void errorCallback(int error, const char* description) {
	fprintf(stderr, "(GLFW) Error [%d]: %s\n", error, description);
}

static void error(int code, const char* description) {
	fprintf(stderr, "Error [%d]: %s", code, description);
	exit(code);
}

void initGLFW_GLAD(GLFWwindow** window) {
	if(!glfwInit()) error(EXIT_GLFW_LOAD_ERROR, "Error initializing GLFW.");

	glfwSetErrorCallback(errorCallback);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	*window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);

	if(!*window) {
		glfwTerminate();
		error(EXIT_GLFW_LOAD_ERROR, "Error creating window.");
	}

	glfwMakeContextCurrent(*window);
	glfwSwapInterval(1);

	if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		error(EXIT_GLAD_LOAD_ERROR, "Error getting GLAD GL loader.");
	}
}