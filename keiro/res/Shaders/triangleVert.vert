#version 400

in vec3 position;

out Attributes
{
	vec3 worldPosition;
	vec2 tex;
} att_out;

uniform float t;
uniform float size = 8.0;

void main()
{
	att_out.worldPosition = position;
	att_out.tex = vec2(position.xz / size + 0.5);
}