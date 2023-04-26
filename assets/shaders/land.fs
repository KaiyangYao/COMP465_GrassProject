#version 410 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture1;

void main() { 
	vec4 color = texture(texture1, TexCoord); 
	// Decrease the saturation of the texture.
	float saturationFactor = 0.5;
	color.rgb *= saturationFactor;
	FragColor = color;
}