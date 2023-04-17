#version 410 core
layout (points) in;
layout (triangle_strip, max_vertices = 36) out;

//in VS_OUT {} gs_in[];

out GS_OUT {
    vec2 textCoord;
} gs_out;
 
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_model;
mat4 rotationY(in float angle);
 
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

	for(int i = 0; i < 4; i++) {
	    gl_Position = u_projection * u_view * (gl_in[0].gl_Position + rotationMat * vertexPosition[i]);
        gs_out.textCoord = textCoords[i];
	    EmitVertex();
    }
    EndPrimitive();
}

void createGrass() {
	createQuad(gl_in[0].gl_Position.xyz, mat4(1.0f));
	createQuad(gl_in[0].gl_Position.xyz, rotationY(radians(45)));
	createQuad(gl_in[0].gl_Position.xyz, rotationY(-radians(45)));
}

mat4 rotationY(in float angle) {
	return mat4(cos(angle),		0,		sin(angle),	    0,
			 		0,		    1.0,		0,	        0,
				-sin(angle),	0,		cos(angle),	    0,
					0, 		    0,			0,	        1);
}
 
void main() {
	createGrass();
} 
 