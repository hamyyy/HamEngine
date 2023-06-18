#version 460 core

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in DATA
{
    vec3 Position;
    vec3 Normal;
    vec3 LocalPosition;
    vec3 LocalNormal;
}
data_in[];

out DATA
{
    vec3 Position;
    vec3 Normal;
    vec3 LocalPosition;
    vec3 LocalNormal;
}
data_out;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform float uTime;
uniform float uResolution;
uniform int uID;
uniform int uIsWireframe;

void main()
{
    for (int i = 0; i < 3; i++)
    {
        int j = i;
        data_out.LocalPosition = data_in[j].LocalPosition;
        data_out.LocalNormal = data_in[j].LocalNormal;
        data_out.Position = vec3(uModel * vec4(data_out.LocalPosition, 1.0f));
        data_out.Normal = mat3(transpose(inverse(uModel))) * data_out.LocalNormal;
        gl_Position = uProjection * uView * vec4(data_out.Position, 1.0);
        EmitVertex();

        data_out.LocalPosition = data_in[j].LocalPosition + data_in[j].LocalNormal * 0.1f;
        data_out.Position = vec3(uModel * vec4(data_out.LocalPosition, 1.0f)) + data_out.Normal * 0.1f;
        gl_Position = uProjection * uView * vec4(data_out.Position, 1.0);
        EmitVertex();

        EndPrimitive();
    }

}