#version 400
layout (vertices = 16) out;

in Attributes
{
	vec3 worldPosition;
	vec2 tex;
} att_in[];

out Attribs
{
	vec3 worldPosition;
	vec2 tex;
} att_out[];

uniform float inner;
uniform float outer;
uniform vec3 cameraPosition;
uniform mat4 viewModel;
float GetZFactor(float depth)
{
	return -8 * log(depth * 0.04)/log(10);
}
void main()
{
	att_out[gl_InvocationID].worldPosition = att_in[gl_InvocationID].worldPosition;
	att_out[gl_InvocationID].tex = att_in[gl_InvocationID].tex;

	if(gl_InvocationID == 0)
	{
		float edge1 = max(1.0, GetZFactor(length(cameraPosition -  (att_in[0].worldPosition + att_in[3].worldPosition)/2)));
		float edge2 = max(1.0, GetZFactor(length(cameraPosition -  (att_in[0].worldPosition + att_in[12].worldPosition)/2)));
		float edge3 = max(1.0, GetZFactor(length(cameraPosition -  (att_in[15].worldPosition + att_in[12].worldPosition)/2)));
		float edge4 = max(1.0, GetZFactor(length(cameraPosition -  (att_in[3].worldPosition + att_in[15].worldPosition)/2)));

		float factor = GetZFactor(length(cameraPosition -  att_in[gl_InvocationID].worldPosition));
		float inn = max(1.0, factor * inner);
		gl_TessLevelInner[0] = inn;
		gl_TessLevelInner[1] = inn;
		
		gl_TessLevelOuter[0] = edge1 * outer;
		gl_TessLevelOuter[1] = edge2 * outer;
		gl_TessLevelOuter[2] = edge3 * outer;
		gl_TessLevelOuter[3] = edge4 * outer;

	}
}