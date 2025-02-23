#version 450 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_colour;

layout (location = 0) out vec3 pass_colour;

void main()
{
	pass_colour = in_colour;

	gl_Position = vec4(in_position, 1.0);
}
