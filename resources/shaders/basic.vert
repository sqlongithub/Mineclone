#version 430 core

layout(std140, binding = 0) uniform FrameData {
    mat4 viewProjection;
    vec3 cameraPosition;
    float time;
};

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 vColor;

uniform mat4 u_Model;

void main()
{
    gl_Position = viewProjection * u_Model * vec4(aPos, 1.0);
    vColor = aColor;
}
