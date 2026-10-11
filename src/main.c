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

char* VERTEX_SOURCE =
"#version 330 core\n"
"layout (location = 0) in vec2 in_position;\n"
"layout (location = 1) in vec3 in_color;\n"
"out vec3 out_color;\n"
"void main() {\n"
"	out_color   = in_color;"
"	gl_Position = vec4(in_position, 0., 1.);"
"}";

char* FRAGMENT_SOURCE =
"#version 330 core\n"
"in vec3 out_color;"
"out vec4 fragColor;"
"void main() {\n"
"	fragColor = vec4(out_color, 1.);"
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

	float vertexData[] = {
		-0.5, -0.5, 1., 0., 0.,
		 0.5, -0.5, 0., 1., 0.,
		 0.0,  0.5, 0., 0., .5
	};

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

	resetTargetVBO();
	resetTargetVAO();

	ShaderProgram program;
	initShaderProgram(&program);

	Shader fragment;
	Shader vertex;
	initShader(&fragment);
	initShader(&vertex);
	loadShaderFromSource(&vertex, SHADER_VERTEX, VERTEX_SOURCE);
	loadShaderFromSource(&fragment, SHADER_FRAGMENT, FRAGMENT_SOURCE);

	attachShaderProgram(&program, &fragment);
	attachShaderProgram(&program, &vertex);

	linkShaderProgram(&program, true);

	glClearColor(0.1f, 0.2f, 0.6f, 1.f);

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
	freeShaderProgram(&program);

	glfwDestroyWindow(window);
	glfwTerminate();
}

static void errorCallback(int error, const char* description) {
	fprintf(stderr, "%s", "(GLFW) Error [%d]: %s\n", error, description);
}

static void error(int code, const char* description) {
	fprintf(stderr, "%s", "Error [%d]: %s", code, description);
	exit(code);
}