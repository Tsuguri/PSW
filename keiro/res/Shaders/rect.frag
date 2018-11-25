#version 400

out vec4 color;

in vec2 pos;

uniform sampler2D texSampler;

void main()
{
	
	vec2 uv = vec2((pos.x+1.0)/2.0, (pos.y+1.0)/2.0);
float z = texture2D(texSampler, uv).r;
	color = vec4(z,z,z,1.0);
	//color = vec4(c,c,c,1.0);
}
