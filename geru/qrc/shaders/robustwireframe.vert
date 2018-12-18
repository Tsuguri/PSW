#version 330 core

in vec3 vertexPosition;
in vec3 vertexNormal;

out vsOut {
    vec3 viewSpacePosition;
    vec3 normal;
} vs_out;

uniform mat4 modelView;
uniform mat3 modelViewNormal;
uniform mat4 mvp;

uniform float minX;
uniform float maxX;
uniform float minY;
uniform float maxY;
uniform float minZ;
uniform float maxZ;

layout (std140) uniform bezier
{
    vec4 bezierPositions[64];
};

vec4 controlPoint(int x, int y, int z) {
    return bezierPositions[x * 4 * 4 + y * 4 + z];
}

vec4 bezierBasis(float t) {
    vec4 B = vec4(1.0, 0.0, 0.0, 0.0);

    for(int i = 1; i < 4; ++i) {
        float saved = 0.0;
        for(int j = 0; j <= i; ++j) {
            float tmp = B[j];
            B[j] = (1.0 - t) * B[j] + saved;
            saved = t * tmp;
        }
    }

    return B;
}

vec4 transformPos(vec3 coords) {
    vec4 result = vec4(0.0, 0.0, 0.0, 0.0);

    vec4 xBasis = bezierBasis(coords.x);
    vec4 yBasis = bezierBasis(coords.y);
    vec4 zBasis = bezierBasis(coords.z);

    for (int x = 0; x < 4; ++x) {
        for(int y = 0; y < 4; ++y) {
            for (int z = 0; z < 4; ++z) {
                result = result + xBasis[x] * yBasis[y] * zBasis[z] * controlPoint(x, y, z);
            }
        }
    }

    return result;
}

void main()
{
	float sx = 1.0 / (maxX - minX);
	float sy = 1.0 / (maxY - minY);
	float sz = 1.0 / (maxZ - minZ);

	mat4 nMatrix = mat4(
		vec4(sx, 0, 0, 0),
		vec4(0, sy, 0, 0),
		vec4(0, 0, sz, 0),
		vec4(-minX * sx, -minY * sy, -minZ * sz, 1)
	);

	mat3 nMatrixNormal = mat3(transpose(inverse(nMatrix)));	

	vec3 viewSpaceNormal = normalize( modelViewNormal * nMatrixNormal * vertexNormal );

	vec4 pos = transformPos(vec3(nMatrix * vec4(vertexPosition, 1)));

	vec4 dPos = transformPos(vec3(nMatrix * vec4(vertexPosition - 0.1 * vertexNormal, 1)));

    vs_out.normal = normalize(modelViewNormal * normalize(vec3(pos - dPos)));
    vs_out.viewSpacePosition = vec3( modelView * pos );

    gl_Position = mvp * pos;
}
