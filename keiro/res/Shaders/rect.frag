#version 400

out vec4 color;

in vec2 pos;

uniform sampler2D texSampler;

void main()
{
	
	vec2 uv = vec2((pos.x+1.0)/2.0, (pos.y+1.0)/2.0);
float z = texture2D(texSampler, uv).r;
float n = 0.1;                                // the near plane
float f = 50.0;                               // the far plane
float c = (2.0 * n) / (f + n - z * (f - n));

	color = vec4(c,c, c,1.0);
}
