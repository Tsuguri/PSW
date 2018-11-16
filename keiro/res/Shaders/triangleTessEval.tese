#version 400
layout (quads, fractional_even_spacing) in;

in Attribs
{
	vec3 worldPosition;
	vec2 tex;
} att_in[];

out Attributes{
	vec3 worldPosition;
	vec3 normal;
	vec2 tex;
	vec3 tangent;
	vec3 binormal;
} att_out;

uniform mat4 viewModel;
uniform mat4 perspective;
mat4 MVP = perspective * viewModel;
uniform sampler2D texHeight;

uniform vec3 cameraPosition;


vec4 EvaluateFunctions(float t)
{
	float v0 = (1-t)*(1-t)*(1-t);
	float v1 = 3*t*(1-t)*(1-t);
	float v2 = 3*t*t*(1-t);
	float v3 = t*t*t;


	return vec4(v0, v1, v2, v3);
}

vec4 EvaluateDerivative(float t)
{
	float v0 = -3 * (1-t) * (1-t);
	float v1 = 3 * (1 - 4 * t + 3 * t * t);
	float v2 = 3 * (2 * t - 3 * t * t);
	float v3 = 3 * t * t;
	return vec4(v0, v1, v2, v3);
}

vec3 EvaluateBspline(vec4 uVal, vec4 vVal)
{
	vec3 result = vec3(0,0,0);

	result += att_in[0].worldPosition * uVal.x * vVal.x;
	result += att_in[4].worldPosition * uVal.y * vVal.x;
	result += att_in[8].worldPosition * uVal.z * vVal.x;
	result += att_in[12].worldPosition * uVal.w * vVal.x;

	result += att_in[1].worldPosition * uVal.x * vVal.y;
	result += att_in[5].worldPosition * uVal.y * vVal.y;
	result += att_in[9].worldPosition * uVal.z * vVal.y;
	result += att_in[13].worldPosition * uVal.w * vVal.y;

	result += att_in[2].worldPosition * uVal.x * vVal.z;
	result += att_in[6].worldPosition * uVal.y * vVal.z;
	result += att_in[10].worldPosition * uVal.z * vVal.z;
	result += att_in[14].worldPosition * uVal.w * vVal.z;

	result += att_in[3].worldPosition * uVal.x * vVal.w;
	result += att_in[7].worldPosition * uVal.y * vVal.w;
	result += att_in[11].worldPosition * uVal.z * vVal.w;
	result += att_in[15].worldPosition * uVal.w * vVal.w;

	return result;
}

void main()
{
	vec2 pos1 = att_in[0].tex * (1-gl_TessCoord.x) + att_in[12].tex * gl_TessCoord.x;
	vec2 pos2 = att_in[3].tex * (1-gl_TessCoord.x) + att_in[15].tex * gl_TessCoord.x;
	att_out.worldPosition = EvaluateBspline(EvaluateFunctions(gl_TessCoord.x), EvaluateFunctions(gl_TessCoord.y));//;
	vec3 uDerivative = 		EvaluateBspline(EvaluateDerivative(gl_TessCoord.x), EvaluateFunctions(gl_TessCoord.y));
	vec3 vDerivative = 		EvaluateBspline(EvaluateFunctions(gl_TessCoord.x), EvaluateDerivative(gl_TessCoord.y));
	att_out.normal = normalize(cross(vDerivative, uDerivative));

	att_out.tex = pos1 * (1-gl_TessCoord.y) + pos2 * gl_TessCoord.y;
	float displacement = textureLod(texHeight, att_out.tex, 1).r * 0.07;
	att_out.worldPosition+=att_out.normal * displacement;
	att_out.tangent = normalize(vDerivative);
	att_out.binormal = normalize(uDerivative);
	gl_Position = MVP * vec4(att_out.worldPosition, 1);
}