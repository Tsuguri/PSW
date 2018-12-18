#version 330 core

uniform struct LightInfo {
    vec4 position;
    vec3 intensity;
} light[2];

uniform vec3 ka;            // Ambient reflectivity
uniform vec3 kd;            // Diffuse reflectivity
uniform vec3 ks;            // Specular reflectivity
uniform float shininess;    // Specular shininess factor

uniform mat4 modelView;

in gsOut {
    vec3 position;
    vec3 normal;
} fs_in;

out vec4 fragColor;

vec3 adsModel( const in vec3 pos, const in vec3 n, vec4 light_position, vec3 light_intensity)
{
    // Calculate the vector from the light to the fragment
    vec3 s = normalize( vec3( modelView * light_position ) - pos );

    // Calculate the vector from the fragment to the eye position (the
    // origin since this is in "eye" or "camera" space
    vec3 v = normalize( -pos );

    // Refleft the light beam using the normal at this fragment
    vec3 r = reflect( -s, n );

    // Calculate the diffus component
    vec3 diffuse = vec3( max( dot( s, n ), 0.0 ) );

    // Calculate the specular component
    vec3 specular = vec3( pow( max( dot( r, v ), 0.0 ), shininess ) );

    // Combine the ambient, diffuse and specular contributions
    return light_intensity * ( ka + kd * diffuse + ks * specular );
}


void main()
{
    vec3 color = vec3(0, 0, 0);
	for (int i = 0; i < 2; ++i)
		color += adsModel(fs_in.position, normalize(fs_in.normal), light[i].position, light[i].intensity);
    fragColor = vec4(color, 1.0);
}
