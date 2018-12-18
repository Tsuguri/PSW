#version 330 core

layout( triangles ) in;
layout( triangle_strip, max_vertices = 3 ) out;

in vsOut {
    vec3 viewSpacePosition;
    vec3 normal;
} gs_in[];

out gsOut {
    vec3 position;
    vec3 normal;
} gs_out;

uniform mat4 modelView;
uniform mat3 modelViewNormal;
uniform mat4 mvp;

uniform mat4 viewportMatrix;

void main()
{
    vec3 normal = normalize(cross(gs_in[1].viewSpacePosition - gs_in[0].viewSpacePosition, gs_in[2].viewSpacePosition - gs_in[0].viewSpacePosition));
    // Vertex 0 (a)
    gs_out.normal = gs_in[0].normal;
    gs_out.position = gs_in[0].viewSpacePosition;
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    // Vertex 1 (b)
    gs_out.normal = gs_in[1].normal;
    gs_out.position = gs_in[1].viewSpacePosition;
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    // Vertex 2 (c)
    gs_out.normal = gs_in[2].normal;
    gs_out.position = gs_in[2].viewSpacePosition;
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    // Finish the primitive off
    EndPrimitive();
}
