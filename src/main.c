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

#define GLAD_LOAD_ERROR 1

static void errorCallback(int error, const char* description);
static void error(int code, const char* description);

char* FRAGMENT_SOURCE =
"#version 330 core\n"
"void main() {\n"
"	gl_FragColor = vec4(1.);"
"}";

char* VERTEX_SOURCE =
"#version 330 core\n"
"in vec2 in_Position;\n"
"void main() {\n"
"	gl_Position = vec4(in_Position, 0., 1.);"
"}";

int main() {
	GLFWwindow* window;

	if(!glfwInit()) return -1;

	glfwSetErrorCallback(errorCallback);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, NULL, NULL);

	if(!window) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		error(GLAD_LOAD_ERROR, "Error getting GLAD GL loader.");
	}

	float vertexData[6] = {
		-0.5, -0.5,
		 0.5, -0.5,
		 0.0,  0.5
	};

	VBO vbo;
	initVBO(&vbo);
	generateVBO(&vbo);
	setTargetVBO(&vbo);
	updateVBO(&vbo, vertexData, sizeof(vertexData));

	VAO vao;
	initVAO(&vao);
	generateVAO(&vao);
	setTargetVAO(&vao);

	updateVAO(&vao);
	resetTargetVBO();
	resetTargetVAO();

	ShaderProgram program;
	initShaderProgram(&program);

	Shader fragment;
	Shader vertex;
	initShader(&fragment);
	initShader(&vertex);
	loadShaderFromSource(&fragment, SHADER_FRAGMENT, FRAGMENT_SOURCE);
	loadShaderFromSource(&vertex, SHADER_VERTEX, VERTEX_SOURCE);

	attachShaderProgram(&program, &fragment);
	attachShaderProgram(&program, &vertex);

	linkShaderProgram(&program);

	glClearColor(1.f, 0.f, 0.f, 1.f);

	while(!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		setTargetVBO(&vbo);
		setTargetVAO(&vao);
		setTargetProgram(&program);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		resetTargetVBO();
		resetTargetVAO();
		resetTargetProgram();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	freeVBO(&vbo);
	freeVAO(&vao);
	freeAttachedShaders(&program);
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