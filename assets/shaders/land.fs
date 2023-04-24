#version 410 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture4;

void main() { 
	FragColor = texture(texture4, TexCoord); 
}