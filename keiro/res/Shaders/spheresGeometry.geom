#version 330 core
layout (points) in;
layout (triangle_strip, max_vertices = 256) out;

in Attributes{
	vec3 worldPosition;
} gs_in[];

uniform float tool;
uniform mat4 mat;

out Atts {
	vec3 worldPosition;
} outElem;

int dr1 = 16;
float dr1f = 16.0;
int dr2 = 8;
float dr2f = 8.0;

void main() {    
	
	vec3  p = gs_in[0].worldPosition;

	for(int j=0;j<dr1;j++) {
		float p1 = j/dr1f * 2 * 3.14;
		float p2 = (j+1)/dr1f * 2 * 3.14;

		float r11 = sin(p1) * tool;
		float r12 = cos(p1) * tool;

		float r21 = sin(p2) * tool;
		float r22 = cos(p2) * tool;

		for(int i=0; i<dr2; i++) {
			float phase = i/dr2f * 2* 3.14;
    			outElem.worldPosition = p + vec3(sin(phase)*r11, cos(phase)*r11 , r12);
			gl_Position = mat*vec4(outElem.worldPosition, 1.0);
			EmitVertex();

    			outElem.worldPosition = p + vec3(sin(phase)*r21, cos(phase)*r21 , r22);
			gl_Position = mat*vec4(outElem.worldPosition, 1.0);
			EmitVertex();
		}
	}
    /*outElem.worldPosition = gs_in[0].worldPosition + vec3(-tool, 0.0, 0.0);
    gl_Position = mat*vec4(outElem.worldPosition, 1.0);
    EmitVertex();

    outElem.worldPosition = gs_in[0].worldPosition + vec3(tool, 0.0, 0.0);
    gl_Position = mat*vec4(outElem.worldPosition, 1.0);
    EmitVertex();*/
    
    EndPrimitive();
}  
