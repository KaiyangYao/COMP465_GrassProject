#version 410 core
layout (points) in;
layout (triangle_strip, max_vertices = 36) out;

//in VS_OUT {} gs_in[];

out GS_OUT {
    vec2 textCoord;
    float colorIndex;
    vec3 fragPos;
    vec3 fragNormal;
} gs_out;
 
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_model;
uniform vec3 u_cameraPosition;

mat4 rotationY(in float angle);
float rand(vec2 co);
 
void createQuad(vec3 basePosition, mat4 rotationMat) {
    // Create the quad of points relative to the base position.
	vec4 vertexPosition[4];
	vertexPosition[0] = vec4(-0.2, 0.0, 0.0, 0.0); 	// down left
	vertexPosition[1] = vec4( 0.2, 0.0, 0.0, 0.0);	// down right
	vertexPosition[2] = vec4(-0.2, 0.2, 0.0, 0.0);	// up left
	vertexPosition[3] = vec4( 0.2, 0.2, 0.0, 0.0);	// up right

    // Add the texture coordinates
    vec2 textCoords[4];
    textCoords[0] = vec2(0.0, 0.0); // down left
    textCoords[1] = vec2(1.0, 0.0); // down right
    textCoords[2] = vec2(0.0, 1.0); // up left
    textCoords[3] = vec2(1.0, 1.0); // up right

    float random = rand(basePosition.xz);
    float randColor = mix(0, 1, random);
    float randAngle = mix(-180.0, 180.0, random);
    float randGrassSize = mix(0.5, 1.5, random);
    mat4 randomRotationMat = rotationY(randAngle);

	for(int i = 0; i < 4; i++) {


	    vec4 worldPosition = gl_in[0].gl_Position + randomRotationMat * rotationMat * (vertexPosition[i]) * randGrassSize;
        gl_Position = u_projection * u_view * worldPosition;
        gs_out.textCoord = textCoords[i];
        gs_out.colorIndex = randColor;
        gs_out.fragPos = worldPosition.xyz;
        gs_out.fragNormal = normalize(mat3(transpose(inverse(u_model))) * vec3(0.0, 0.0, 1.0));
		EmitVertex();
    }
    EndPrimitive();
}

// referenced from tutorial and changed rotations
void createGrass(int quadsNum) {
	switch(quadsNum) {
		case 1: {
			createQuad(gl_in[0].gl_Position.xyz, mat4(1.0));
			break;
		}
		case 2: {
			createQuad(gl_in[0].gl_Position.xyz, rotationY(radians(45)));
			createQuad(gl_in[0].gl_Position.xyz, rotationY(-radians(45)));
			break;
		}
		case 3: {
			createQuad(gl_in[0].gl_Position.xyz, mat4(1.0));
			createQuad(gl_in[0].gl_Position.xyz, rotationY(radians(45)));
			createQuad(gl_in[0].gl_Position.xyz, rotationY(-radians(45)));
			break;
		}
	}

}

// UITLS
mat4 rotationY(in float angle) {
	return mat4(cos(angle),		0,		sin(angle),	    0,
			 		0,		    1.0,		0,	        0,
				-sin(angle),	0,		cos(angle),	    0,
					0, 		    0,			0,	        1);
}

// simple pseudo-random number generator to generate a random number between 0 and 1.
float rand(vec2 co) {
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}
 
void main() {
	const float LOD1 = 2.0f;
	const float LOD2 = 4.0f;
	const float LOD3 = 8.0f;
	float dist =  length(gl_in[0].gl_Position.xyz - u_cameraPosition);
	// distance of position to camera
	float randomF = mix(0, 1,rand(gl_in[0].gl_Position.xz));
	float t = 8.0f; 
	if (dist > LOD2) t *= 1.5f;
	dist += (randomF*t - t/2.0f);

	// change number of quad function of distance
	int lessDetails = 3;
	if (dist > LOD1) lessDetails = 2;
	if (dist > LOD2) lessDetails = 1;
	if (dist > LOD3) lessDetails = 0;

	// create grass
	// reference from tutorial here
	if (lessDetails != 1
		|| (lessDetails == 1 && (int(gl_in[0].gl_Position.x * 10) % 1) == 0 || (int(gl_in[0].gl_Position.z * 10) % 1) == 0)
		|| (lessDetails == 2 && (int(gl_in[0].gl_Position.x * 5) % 1) == 0 || (int(gl_in[0].gl_Position.z * 5) % 1) == 0)
	)
		createGrass(lessDetails);
} 
 