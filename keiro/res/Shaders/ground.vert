#version 140

in vec3 position;

uniform mat4 mat;

void main()
{
	vec4 pos = mat*vec4(position, 1.0);
	gl_Position = pos;
}
