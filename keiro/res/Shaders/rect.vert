#version 400

out vec2 pos;

in vec3 position;


void main()
{
	pos = position.xy;
	gl_Position = vec4(position, 1.0);
}
