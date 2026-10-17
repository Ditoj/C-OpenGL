#version 460 core
layout (location = 0) in vec2 in_position;
layout (location = 1) in vec3 in_color;
layout (location = 2) in vec2 in_texCoord;
out vec3 out_color;
out vec2 out_texCoord;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

void main() {
	out_color    = in_color;
	out_texCoord = in_texCoord;
	gl_Position  = u_proj * u_view * u_model * vec4(in_position, 0., 1.);
}