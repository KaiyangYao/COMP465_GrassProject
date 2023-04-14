#version 410 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;
// in VS_OUT {} gs_in[];
// out GS_OUT {} gs_out;
 
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_model;
 
void createQuad(vec4 base_position){
	vec4 vertexPosition[4];
	vertexPosition[0] = vec4(-0.2, 0.0, 0.0, 0.0); 	// down left
	vertexPosition[1] = vec4( 0.2, 0.0, 0.0, 0.0);	// down right
	vertexPosition[2] = vec4(-0.2, 0.2, 0.0, 0.0);	// up left
	vertexPosition[3] = vec4( 0.2, 0.2, 0.0, 0.0);	// up right
	for(int i = 0; i < 4; i++) {
	    gl_Position = u_projection * u_view * (base_position + vertexPosition[i]);
	    EmitVertex();
    }
    EndPrimitive();
}
 
void main(){
	createQuad(gl_in[0].gl_Position);
} 
 