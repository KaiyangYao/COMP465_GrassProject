#version 410 core
out vec4 FragColor;

in GS_OUT {
    vec2 textCoord;
	float colorIndex;
} fs_in;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;

void main() {
	vec4 color;
    if (fs_in.colorIndex < 0.25) {
        color = texture(texture1, fs_in.textCoord);
    } else if (fs_in.colorIndex < 0.5) {
        color = texture(texture2, fs_in.textCoord);
    } else if (fs_in.colorIndex < 0.75) {
		color = texture(texture3, fs_in.textCoord);
	} else {
		color = texture(texture4, fs_in.textCoord);
	}
	
    if (color.a < 0.05) discard;
    FragColor = color;
}