#version 430 core

in vec2 vUV;
in vec3 vNormal;
in vec3 vFragPos;
in vec3 vColor;
flat in uint vTexIndex;

out vec4 FragColor;

uniform sampler2DArray u_TextureArray;
uniform vec3 u_LightDir = normalize(vec3(-0.5, -1.0, -0.3)); // simple directional light

void main()
{
    FragColor = texture(u_TextureArray, vec3(vUV, float(vTexIndex)));
}
