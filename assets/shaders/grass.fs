#version 410 core
out vec4 FragColor;

in GS_OUT {
    vec2 textCoord;
} fs_in;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main() {
	vec4 color1 = texture(texture0, fs_in.textCoord);
	vec4 color2 = texture(texture1, fs_in.textCoord);
	if (color1.a < 0.05 || color2.a < 0.05) discard; // remove the transparent areas so that our herbs are displayed correctly
	FragColor = color2;
	// if (fs_in.textCoord[0].x < 0.5) {
	// 	FragColor = color1;
	// } else {
	// 	FragColor = color2;
	// }
}