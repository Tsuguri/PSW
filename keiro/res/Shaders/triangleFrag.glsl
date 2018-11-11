#version 140

out vec4 color;

in vec2 texCoord;

uniform sampler2D texSampler;

void main()
{
	color = vec4(1,0,0,1);
}