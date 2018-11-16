#version 400

out vec4 color;

in vec3 worldPos;

uniform sampler2D texSampler;

void main()
{
	color = vec4(1,1,0,1);
}