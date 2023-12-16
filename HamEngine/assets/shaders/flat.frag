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
    if (uIsWireframe == 1)
    {
        FragColor = vec4(uWireframeColor, 1.0);
        return;
    }
    FragColor = vec4(vec3(1.0, 0.5, 0.06), 1.0);
}
