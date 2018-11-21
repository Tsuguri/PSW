#version 400
layout (quads, fractional_even_spacing) in;

in Attribs
{
	vec3 worldPosition;
	vec2 tex;
} att_in[];

out Attributes{
	vec3 worldPosition;
	vec3 pos;
	vec3 normal;
	vec2 tex;
	vec3 tangent;
	vec3 binormal;
} att_out;

uniform mat4 viewModel;
uniform mat4 perspective;
mat4 MVP = perspective * viewModel;
uniform sampler2D texHeight;
uniform int x;
uniform int y;
uniform float tool;

uniform vec3 cameraPosition;



float BSplineN0(float i, float t)
{
	return (t>=i -1 && t<i) ? 1.0 : 0.0;
}

float BSplineMix(float n, float i, float t, float N1, float N2)
{
	return ((t-i+1)*N1 + (i+n-t)*N2) / n;
}


vec4 EvaluateFunctions(float t)
{
	float v0 = BSplineN0(-2,t);
	float v1 = BSplineN0(-1,t);
	float v2 = BSplineN0(0,t);
	float v3 = BSplineN0(1,t);
	float v4 = BSplineN0(2,t);
	float v5 = BSplineN0(3,t);
	float v6 = BSplineN0(4,t);

	v0 = BSplineMix(1, -2, t, v0, v1);
	v1 = BSplineMix(1, -1, t, v1, v2);
	v2 = BSplineMix(1, 0, t, v2, v3);
	v3 = BSplineMix(1, 1, t, v3, v4);
	v4 = BSplineMix(1, 2, t, v4, v5);
	v5 = BSplineMix(1, 3, t, v5, v6);

	v0 = BSplineMix(2, -2, t, v0, v1);
	v1 = BSplineMix(2, -1, t, v1, v2);
	v2 = BSplineMix(2, 0, t, v2, v3);
	v3 = BSplineMix(2, 1, t, v3, v4);
	v4 = BSplineMix(2, 2, t, v4, v5);

	v0 = BSplineMix(3, -2, t, v0, v1);
	v1 = BSplineMix(3, -1, t, v1, v2);
	v2 = BSplineMix(3, 0, t, v2, v3);
	v3 = BSplineMix(3, 1, t, v3, v4);

	return vec4(v0, v1, v2, v3);
}

vec3 EvaluateDerivative(float t)
{
	float v0 =BSplineN0(-1, t);
	float v1 = BSplineN0(+0, t);
	float v2 = BSplineN0(+1, t);
	float v3 = BSplineN0(+2, t);
	float v4 = BSplineN0(+3, t);

	v0 = BSplineMix(1, -1, t, v0, v1);
	v1 = BSplineMix(1, +0, t, v1, v2);
	v2 = BSplineMix(1, +1, t, v2, v3);
	v3 = BSplineMix(1, +2, t, v3, v4);

	v0 = BSplineMix(2, -1, t, v0, v1);
	v1 = BSplineMix(2, +0, t, v1, v2);
	v2 = BSplineMix(2, +1, t, v2, v3);

	return vec3(v0, v1, v2);
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

vec3 EvalueateUDerivative(float u, float v)
{
	vec3 uVal = EvaluateDerivative(u);
	vec4 vVal = EvaluateFunctions(v);
	vec3 result = vec3(0,0,0);

	result += (att_in[4].worldPosition - att_in[0].worldPosition) * uVal.x * vVal.x;
	result += (att_in[8].worldPosition - att_in[4].worldPosition) * uVal.y * vVal.x;
	result += (att_in[12].worldPosition - att_in[8].worldPosition) * uVal.z * vVal.x;
	//result += att_in[12].worldPosition * uVal.w * vVal.x;

	result += (att_in[5].worldPosition - att_in[1].worldPosition) * uVal.x * vVal.y;
	result += (att_in[9].worldPosition - att_in[5].worldPosition) * uVal.y * vVal.y;
	result += (att_in[13].worldPosition - att_in[9].worldPosition) * uVal.z * vVal.y;
	//result += att_in[13].worldPosition * uVal.w * vVal.y;

	result += (att_in[6].worldPosition - att_in[2].worldPosition) * uVal.x * vVal.z;
	result += (att_in[10].worldPosition - att_in[6].worldPosition) * uVal.y * vVal.z;
	result += (att_in[14].worldPosition - att_in[10].worldPosition) * uVal.z * vVal.z;
	//result += att_in[14].worldPosition * uVal.w * vVal.z;

	result += (att_in[7].worldPosition - att_in[3].worldPosition) * uVal.x * vVal.w;
	result += (att_in[11].worldPosition - att_in[7].worldPosition) * uVal.y * vVal.w;
	result += (att_in[15].worldPosition - att_in[11].worldPosition) * uVal.z * vVal.w;
	//result += att_in[15].worldPosition * uVal.w * vVal.w;

	return result;
}

vec3 EvalueateVDerivative(float u, float v)
{
	vec4 uVal = EvaluateFunctions(u);
	vec3 vVal = EvaluateDerivative(v);
	vec3 result = vec3(0,0,0);

	result += (-att_in[0].worldPosition + att_in[1].worldPosition) * uVal.x * vVal.x;
	result += (-att_in[4].worldPosition + att_in[5].worldPosition) * uVal.y * vVal.x;
	result += (-att_in[8].worldPosition + att_in[9].worldPosition) * uVal.z * vVal.x;
	result += (-att_in[12].worldPosition + att_in[13].worldPosition) * uVal.w * vVal.x;

	result += (-att_in[1].worldPosition + att_in[2].worldPosition) * uVal.x * vVal.y;
	result += (-att_in[5].worldPosition + att_in[6].worldPosition) * uVal.y * vVal.y;
	result += (-att_in[9].worldPosition + att_in[10].worldPosition) * uVal.z * vVal.y;
	result += (-att_in[13].worldPosition + att_in[14].worldPosition) * uVal.w * vVal.y;

	result += (-att_in[2].worldPosition + att_in[3].worldPosition) * uVal.x * vVal.z;
	result += (-att_in[6].worldPosition + att_in[7].worldPosition) * uVal.y * vVal.z;
	result += (-att_in[10].worldPosition + att_in[11].worldPosition) * uVal.z * vVal.z;
	result += (-att_in[14].worldPosition + att_in[15].worldPosition) * uVal.w * vVal.z;

	return result;
}


float GetZFactor(float depth)
{
	return -16* log(depth * 0.01)/log(10);
}

void main()
{
	att_out.worldPosition = EvaluateBspline(EvaluateFunctions(gl_TessCoord.x), EvaluateFunctions(gl_TessCoord.y));//;
	vec3 uDerivative = 		EvalueateUDerivative(gl_TessCoord.x, gl_TessCoord.y);
	vec3 vDerivative = 		EvalueateVDerivative(gl_TessCoord.x, gl_TessCoord.y);
	att_out.normal = normalize(cross(vDerivative, uDerivative));

	att_out.tex = (gl_TessCoord.xy + vec2(x,y))/8 ;// pos1 * (1-gl_TessCoord.y) + pos2 * gl_TessCoord.y;

	vec3 pos = (viewModel * vec4(att_out.worldPosition, 1)).xyz;
	float level = 6 - GetZFactor(pos.z);
	pos = att_out.worldPosition + att_out.normal*tool;
	att_out.tangent = normalize(vDerivative);
	att_out.binormal = normalize(uDerivative);
	gl_Position = MVP * vec4(pos, 1);
	att_out.pos = gl_Position.xyz;
}
