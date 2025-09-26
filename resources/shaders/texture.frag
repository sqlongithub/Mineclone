#version 430 core

in vec2 vUV;
in vec3 vNormal;
in vec3 vFragPos;
in vec3 vTintColor;
flat in uint vTexIndex;

out vec4 FragColor;

uniform sampler2DArray uTextureArray;
uniform vec3 uLightDir = normalize(vec3(-0.5, -1.0, -0.3));

void main()
{
    vec4 texColor = texture(uTextureArray, vec3(vUV, vTexIndex));
    FragColor = vec4(texColor.rgb * vTintColor, texColor.a);
}
