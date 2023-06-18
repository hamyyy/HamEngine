#version 460 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;

out DATA
{
    vec3 Position;
    vec3 Normal;
    vec3 LocalPosition;
    vec3 LocalNormal;
} data_out;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform float uTime;
uniform float uResolution;
uniform int uID;
uniform int uIsWireframe;


void main()
{
	data_out.Position = vec3(uModel * vec4(aPosition, 1.0f));
	data_out.Normal = mat3(transpose(inverse(uModel))) * aNormal;
    data_out.LocalPosition = aPosition;
    data_out.LocalNormal = aNormal;

    if (uIsWireframe == 1) // move vertices towards camera to avoid z-fighting
    {
        vec3 viewDir = (data_out.Position - vec3(inverse(uView)[3]));
        // FragPos += viewDir * 0.001;
        data_out.Position -= viewDir * 0.001;
    }

    gl_Position = uProjection * uView * vec4(data_out.Position, 1.0);
}