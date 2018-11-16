#version 400

out vec4 color;

in Attributes
{
	vec3 worldPosition;
	vec3 normal;
	vec2 tex;
	vec3 tangent;
	vec3 binormal;
} att_in;

uniform sampler2D texDiffuse;
uniform sampler2D texNormal;

uniform vec3 lightPosition = vec3(2.0, 2.0, 2.0);

uniform vec3 cameraPosition;

float ambientStrength = 0.3;

vec3 lightColor = vec3(0.9, 0.9, 0.9);
vec3 defaultColor = vec3(0.3, 0.3, 0.3);
float specularStrength = 1;
int materialShininess = 64;
float gamma = 2.2;
void main()
{
	vec3 colorTmp = texture(texDiffuse, att_in.tex).rgb;
	colorTmp = pow(colorTmp, vec3(gamma));
	vec3 norm = texture(texNormal, att_in.tex).rgb;
	norm = pow(norm, vec3(gamma));
	norm = normalize(norm.x * att_in.tangent + norm.y * att_in.binormal + norm.z * att_in.normal);
	// Ambient
	vec3 ambient = ambientStrength * lightColor;

	// Diffuse
	vec3 dir = normalize(lightPosition - att_in.worldPosition);
	float diff = max(dot(dir, norm), 0.0);
	vec3 diffuse = diff * lightColor;
		

	// Specular
	vec3 viewDir = normalize(cameraPosition - att_in.worldPosition);
	vec3 halfwayDir = normalize(dir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), materialShininess);
	vec3 specular = lightColor * spec * specularStrength;
	vec3 result = (ambient + diffuse) * colorTmp + specular;
	//result = pow(result, vec3(1.0 / gamma));
	color = vec4(result, 1.0);
	//color = vec4(norm, 1.0);
}