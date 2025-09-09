#version 330 core

layout (location = 0) in vec3 aPos;   // from Vertex.position
layout (location = 1) in vec3 aColor; // from Vertex.color

out vec3 vColor; // pass to fragment shader

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * vec4(aPos, 1.0);
    vColor = aColor;
}
