#version 330 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 texCoords;

void main()
{
	gl_Position = vec4(0.9*pos.x, 0.9*pos.y, 1.0, 1.0);
}