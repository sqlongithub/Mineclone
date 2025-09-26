#version 430 core

layout(std140, binding = 0) uniform FrameData {
    mat4 viewProjection;
    vec3 cameraPosition;
    float time;
};

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 2) in uint aFaceIndex; // face index (0-5)
layout(location = 3) in uint aTextureIndex;
layout(location = 4) in uint aTintIndex;

out vec2 vUV;
out vec3 vNormal;
out vec3 vFragPos;
out vec3 vTintColor;
flat out uint vTexIndex;

uniform mat4 uModel;
uniform vec3 uBiomeTints[256];

const vec3 FACE_NORMALS[6] = vec3[](
    vec3(0.0, 0.0, 1.0),   // NORTH (+Z)
    vec3(0.0, 0.0, -1.0),  // SOUTH (-Z)
    vec3(1.0, 0.0, 0.0),   // EAST (+X)
    vec3(-1.0, 0.0, 0.0),  // WEST (-X)
    vec3(0.0, 1.0, 0.0),   // UP (+Y)
    vec3(0.0, -1.0, 0.0)   // DOWN (-Y)
);

void main()
{
    vUV = aUV;

    vNormal = FACE_NORMALS[int(aFaceIndex)];

    vFragPos = vec3(uModel * vec4(aPos, 1.0));

    vTexIndex = aTextureIndex;

    if (aTintIndex == 255u) {
        vTintColor = vec3(1.0);
    } else {
        vTintColor = uBiomeTints[aTintIndex];
    }

    gl_Position = viewProjection * uModel * vec4(aPos, 1.0);
}
