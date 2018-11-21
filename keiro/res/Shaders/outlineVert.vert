#version 400

in vec3 position;

out Attributes
{
	vec3 worldPosition;
} att_out;

uniform float t;
uniform mat4 mat;

void main()
{
	att_out.worldPosition = position;
	vec3 pos = position;
	gl_Position = mat * vec4(pos, 1.0);
}
