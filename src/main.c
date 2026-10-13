#include "common/common.h"
#include "glad/glad.h"
#include "glfw/glfw3.h"
#include "vertex_buffer/vertex_buffer.h"
#include "vertex_array/vertex_array.h"
#include "shader/shader.h"

#include <stdio.h>
#include <stdlib.h>

#define WINDOW_WIDTH  640
#define WINDOW_HEIGHT 480
#define WINDOW_TITLE  "DitoJ"

#define EXIT_GLFW_LOAD_ERROR 1
#define EXIT_GLAD_LOAD_ERROR 2

static const float vertexData[] = {
	//x     y   r   g   b
	-0.5, -0.5, 1., 0., 0.,
	 0.5, -0.5, 0., 1., 0.,
	 0.0,  0.5, 0., 0., .5
};

static void errorCallback(int error, const char* description);
static void error(int code, const char* description);

void initGLFW_GLAD(GLFWwindow** window);

int main() {
	GLFWwindow* window;
	initGLFW_GLAD(&window);

	VBO vbo;
	initVBO(&vbo);
	setTargetVBO(&vbo);
	sendDataVBO(vertexData, sizeof(vertexData));

	VAO vao;
	initVAO(&vao);
	setTargetVAO(&vao);
	addAttributeVAO(&vao, 2, GL_FLOAT);
	addAttributeVAO(&vao, 3, GL_FLOAT);
	patchVAO(&vao);

	ShaderProgram program;
	initShaderProgram(&program);

	Shader fragment;
	Shader vertex;
	initShader(&fragment);
	initShader(&vertex);
	loadShaderFromFile(&vertex, SHADER_VERTEX, "../shader/base/base.vsh");
	loadShaderFromFile(&fragment, SHADER_FRAGMENT, "../shader/base/base.fsh");

	attachShaderProgram(&program, &fragment);
	attachShaderProgram(&program, &vertex);

	linkShaderProgram(&program, true);
	setTargetProgram(&program);

	glClearColor(0.1f, 0.2f, 0.6f, 1.f);

	while(!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	freeVBO(&vbo);
	freeVAO(&vao);
	freeShaderProgram(&program);

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