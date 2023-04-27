#version 410 core
layout (points) in;
layout (triangle_strip, max_vertices = 36) out;

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
uniform float u_time;
uniform sampler2D u_wind;

mat4 rotationY(in float angle);
mat4 rotationX(in float angle);
mat4 rotationZ(in float angle);
float rand(vec2 co);

const float pi = 3.14;
 
void createQuad(vec3 basePosition, mat4 dir) {
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

    // wind

	vec2 w_direction = vec2(0.5, 0.5); 
	float w_strength = 0.05f;
	vec2 uv = basePosition.xz + w_direction * w_strength * u_time ;
	uv.x = mod(uv.x,1.0);
	uv.y = mod(uv.y,1.0);
	vec4 w = texture(u_wind, uv);
	mat4 wind_effect =  (rotationX(w.x*pi*0.75f - pi*0.25f) * rotationZ(w.y*pi*0.75f - pi*0.25f));
	mat4 wind_apply = mat4(1);

	for(int i = 0; i < 4; i++) {
		// only apply effect on the top of texture
		if (i == 2) wind_apply = wind_effect;
	    vec4 worldPosition = gl_in[0].gl_Position + wind_apply * dir * randomRotationMat * (vertexPosition[i]) * randGrassSize;
        gl_Position = u_projection * u_view * worldPosition;
        gs_out.textCoord = textCoords[i];
        gs_out.colorIndex = randColor;
        gs_out.fragPos = worldPosition.xyz;
        gs_out.fragNormal = normalize(mat3(transpose(inverse(u_model))) * vec3(0.0, 0.0, 1.0));
		EmitVertex();
    }
    EndPrimitive();
}


// ------------------------------- UITLS -------------------------------
mat4 rotationX( in float angle ) {
	return mat4(	1.0,		0,			0,			0,
			 		0, 	cos(angle),	-sin(angle),		0,
					0, 	sin(angle),	 cos(angle),		0,
					0, 			0,			  0, 		1);
}

mat4 rotationY( in float angle )
{
	return mat4(	cos(angle),		0,		sin(angle),	0,
			 				0,		1.0,			 0,	0,
					-sin(angle),	0,		cos(angle),	0,
							0, 		0,				0,	1);
}

mat4 rotationZ( in float angle ) {
	return mat4(	cos(angle),		-sin(angle),	0,	0,
			 		sin(angle),		cos(angle),		0,	0,
							0,				0,		1,	0,
							0,				0,		0,	1);
}

// simple pseudo-random number generator to generate a random number between 0 and 1.
float rand(vec2 co) {
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

// helper to create grass based on the num of quads.
void createGrass(int numQuads) {
	if (numQuads == 3) {
		createQuad(gl_in[0].gl_Position.xyz, rotationY(0));
		createQuad(gl_in[0].gl_Position.xyz, rotationY(pi/3));
		createQuad(gl_in[0].gl_Position.xyz, rotationY(-pi/3));
	} else if (numQuads == 2) {
		createQuad(gl_in[0].gl_Position.xyz, rotationY(0));
		createQuad(gl_in[0].gl_Position.xyz, rotationY(pi/2));
	} else if (numQuads == 1) {
		createQuad(gl_in[0].gl_Position.xyz, rotationY(0));
	}
}
// ---------------------------------------------------------------------
 

void main() {
	// LOD distance definations
	const float LOD1 = 1.0f;
	const float LOD2 = 2.0f;
	const float LOD3 = 4.0f;

	float dist = length(gl_in[0].gl_Position.xyz - u_cameraPosition);

	// feather the edge between level of details.
	float randomFactor = 8.0f; 
	dist += randomFactor * mix(-1, 1, rand(gl_in[0].gl_Position.xz));

	// change number of quad function of distance
	int numQuads = 3;
	if (dist > LOD1) numQuads = 2;
	if (dist > LOD2) numQuads = 1;
	if (dist > LOD3) numQuads = 0;
	
	createGrass(numQuads);
} 
 