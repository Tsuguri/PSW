#version 140

in vec3 position;
in vec3 normal;
in vec2 tex;

out vec2 texCoord;

uniform float t;
uniform mat4 mat;

void main()
{
	vec4 pos = mat*vec4(position, 1.0);
	texCoord = tex;
	gl_Position = pos;
}