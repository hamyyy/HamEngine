#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 LocalFragPos;
out vec3 Normal;
out vec3 LocalNormal;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform float uTime;
uniform float uResolution;
uniform int uID;
uniform int uIsWireframe;

void main()
{
    FragPos = vec3(uModel * vec4(aPosition, 1.0));
    Normal = mat3(transpose(inverse(uModel))) * aNormal;
    LocalNormal = aNormal;
    LocalFragPos = aPosition;

    if (uIsWireframe == 1) // move vertices towards camera to avoid z-fighting
    {
        vec3 viewDir = (FragPos - vec3(inverse(uView)[3]));
        // FragPos += viewDir * 0.001;
        FragPos -= viewDir * 0.001;
    }

    gl_Position = uProjection * uView * vec4(FragPos, 1.0);
}
