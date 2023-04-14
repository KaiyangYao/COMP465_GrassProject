#version 410 core
out vec4 FragColor;

in GS_OUT {
    vec2 textCoord;
} fs_in;

uniform sampler2D u_textgrass;

void main(){
	vec4 color = texture(u_textgrass, fs_in.textCoord);
	if (color.a < 0.05 ) discard; // remove the transparent areas so that our herbs are displayed correctly
	FragColor = color;
}