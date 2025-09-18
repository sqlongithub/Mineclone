#version 430 core

layout(std140, binding = 0) uniform FrameData {
    mat4 viewProjection;
    vec3 cameraPosition;
    float time;
};

layout(location = 0) in vec3 aPos;    // vertex position
layout(location = 1) in vec3 aColor;  // optional vertex color
layout(location = 2) in vec3 aNormal; // vertex normal
layout(location = 3) in vec2 aUV;     // texture coordinates
layout(location = 4) in uint aTextureIndex;

out vec2 vUV;
out vec3 vNormal;
out vec3 vFragPos;
out vec3 vColor;
flat out uint vTexIndex;

uniform mat4 u_Model;

void main()
{
    vUV = aUV;
    vNormal = mat3(transpose(inverse(u_Model))) * aNormal; // normal in world space
    vFragPos = vec3(u_Model * vec4(aPos, 1.0));
    vColor = aColor;
    vTexIndex = aTextureIndex;
    gl_Position = viewProjection * u_Model * vec4(aPos, 1.0);

}
