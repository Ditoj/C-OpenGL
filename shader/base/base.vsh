#version 460 core
layout (location = 0) in vec2 in_position;
layout (location = 1) in vec3 in_color;
layout (location = 2) in vec2 in_texCoord;
out vec3 out_color;
out vec2 out_texCoord;

void main() {
	out_color    = in_color;
	out_texCoord = in_texCoord;
	gl_Position  = vec4(in_position, 0., 1.);
}