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

    vec3 posNorm = max(vec3(0.0), data_in.LocalNormal);
    vec3 negNorm = min(data_in.LocalNormal, vec3(0.0));
    vec3 normColor = posNorm + abs(negNorm) * 0.7;

    FragColor = vec4(normColor * uObjectColor, 1.0);
}
