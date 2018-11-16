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
	return -16* log(depth * 0.01)/log(10);
}

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
	att_out[gl_InvocationID].worldPosition = att_in[gl_InvocationID].worldPosition;
	att_out[gl_InvocationID].tex = att_in[gl_InvocationID].tex;

	if(gl_InvocationID == 0)
	{
		vec4 pos1 = viewModel * vec4(EvaluateBspline(EvaluateFunctions(0),EvaluateFunctions(0.5)), 1.0);
		vec4 pos2 = viewModel * vec4(EvaluateBspline(EvaluateFunctions(0.5),EvaluateFunctions(1)), 1.0);
		vec4 pos3 = viewModel * vec4(EvaluateBspline(EvaluateFunctions(1),EvaluateFunctions(0.5)), 1.0);
		vec4 pos4 = viewModel * vec4(EvaluateBspline(EvaluateFunctions(0.5),EvaluateFunctions(0)), 1.0);

		vec4 centralPos = viewModel * vec4(EvaluateBspline(EvaluateFunctions(0.5),EvaluateFunctions(0.5)), 1.0);

		float edge1 = max(1.0, GetZFactor(pos1.z) / 2);
		float edge4 = max(1.0, GetZFactor(pos2.z) / 2);
		float edge3 = max(1.0, GetZFactor(pos3.z) / 2);
		float edge2 = max(1.0, GetZFactor(pos4.z) / 2);

		float factor = GetZFactor(centralPos.z) / 2;
		float inn = max(1.0, factor * inner);
		gl_TessLevelInner[0] = inn;
		gl_TessLevelInner[1] = inn;
		
		gl_TessLevelOuter[0] = edge1 * outer;
		gl_TessLevelOuter[1] = edge2 * outer;
		gl_TessLevelOuter[2] = edge3 * outer;
		gl_TessLevelOuter[3] = edge4 * outer;

	}
}