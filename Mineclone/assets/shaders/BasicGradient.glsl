#type vertex

#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 color;

out vec3 vertexColor;

void main() {
	gl_Position = position;
	vertexColor = color;
}



#type fragment

#version 330 core

layout(location = 0) out vec4 color;

in vec3 vertexColor;

void main() {
	color = vec4(vertexColor, 1.0);
}