#version 460 core

in DATA
{
    vec3 Position;
    vec3 Normal;
    vec3 LocalPosition;
    vec3 LocalNormal;
}
data_in;

out vec4 FragColor;

uniform int uID;
uniform int uTotalObjects;

void main()
{
    float r = (uID & 0xFF) / 255.0;
    float g = (uID >> 8 & 0xFF) / 255.0;
    float b = (uID >> 16 & 0xFF) / 255.0;
    float a = (uID >> 24 & 0xFF) / 255.0;

    FragColor = vec4(r, g, b, a);
}
