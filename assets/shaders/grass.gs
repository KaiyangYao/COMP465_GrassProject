#version 410 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;
// in VS_OUT {} gs_in[];
out GS_OUT {
    vec2 textCoord;
} gs_out;
 
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_model;
 
void createQuad(vec4 base_position){
	vec4 vertexPosition[4];
	vertexPosition[0] = vec4(-0.2, 0.0, 0.0, 0.0); 	// down left
	vertexPosition[1] = vec4( 0.2, 0.0, 0.0, 0.0);	// down right
	vertexPosition[2] = vec4(-0.2, 0.2, 0.0, 0.0);	// up left
	vertexPosition[3] = vec4( 0.2, 0.2, 0.0, 0.0);	// up right

    vec2 textCoords[4];
    textCoords[0] = vec2(0.0, 0.0); // down left
    textCoords[1] = vec2(1.0, 0.0); // down right
    textCoords[2] = vec2(0.0, 1.0); // up left
    textCoords[3] = vec2(1.0, 1.0); // up right

	for(int i = 0; i < 4; i++) {
	    gl_Position = u_projection * u_view * (base_position + vertexPosition[i]);
        gs_out.textCoord = textCoords[i];
	    EmitVertex();
    }
    EndPrimitive();
}
 
void main(){
	createQuad(gl_in[0].gl_Position);
} 
 