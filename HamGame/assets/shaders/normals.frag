#version 460 core

in vec3 FragPos;
in vec3 LocalFragPos;
in vec3 Normal;
in vec3 LocalNormal;

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
    
    FragColor = vec4(LocalNormal * uObjectColor, 1.0);
}
