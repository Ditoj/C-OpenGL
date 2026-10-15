#version 460 core
in vec3 out_color;
in vec2 out_texCoord;
out vec4 fragColor;

uniform sampler2D u_texture;

void main() {
	fragColor = vec4(texture(u_texture, out_texCoord).rgb, 1.) * vec4(out_color, 1.);
}