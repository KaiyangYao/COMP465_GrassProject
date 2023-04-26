#version 410 core
out vec4 FragColor;

in GS_OUT {
    vec2 textCoord;
	float colorIndex;
	vec3 fragPos;
    vec3 fragNormal;
} fs_in;

uniform sampler2D texture0;

vec3 lightPos = vec3(0.0, 0.0, 0.0);
vec3 lightColor = vec3(1.0, 1.0, 1.0);
float ambientStrength = 0.2;
float specularStrength = 0.5;
float shininess = 30.0;

void main() {
	vec4 color;
    // if (fs_in.colorIndex < 0.25) {
    //     color = texture(texture0, fs_in.textCoord);
    // } else if (fs_in.colorIndex < 0.5) {
    //     color = texture(texture1, fs_in.textCoord);
    // } else if (fs_in.colorIndex < 0.75) {
	// 	color = texture(texture2, fs_in.textCoord);
	// } else {
	// 	color = texture(texture3, fs_in.textCoord);
	// }
	// if (fs_in.colorIndex < 0.95) {
	// 	color = texture(texture0, fs_in.textCoord);
	// } else {
	// 	color = texture(texture1, fs_in.textCoord);
	// }
	
	color = texture(texture0, fs_in.textCoord);
	
    if (color.a < 0.6) discard;
    // Compute the diffuse lighting contribution
	vec3 ambient = ambientStrength * lightColor;
	vec3 lightDir = normalize(lightPos - fs_in.fragPos);
	float diff = max(dot(fs_in.fragNormal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	
	// Compute the specular lighting contribution
	vec3 viewDir = normalize(-fs_in.fragPos);
	vec3 reflectDir = reflect(-lightDir, fs_in.fragNormal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specular = specularStrength * spec * lightColor;
	
	// Combine the lighting and texture color
	vec3 result = (ambient + diffuse + specular) * color.rgb;
    // FragColor = vec4(result, color.a);
	FragColor = color;
}