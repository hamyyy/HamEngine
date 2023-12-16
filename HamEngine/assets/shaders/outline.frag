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

uniform vec3 uLightPos;
uniform vec3 uLightColor;
uniform vec3 uObjectColor;
uniform vec3 uWireframeColor;
uniform float uTime;
uniform float uResolution;
uniform int uIsWireframe;

void main()
{
    vec3 color = vec3(1.0, 0.5, 0.0);
    FragColor = vec4(color, 1.0);
}
