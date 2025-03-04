#version 450 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_colour;
layout (location = 2) in vec2 in_uvCoords;

layout (location = 0) out vec3 pass_colour;
layout (location = 1) out vec2 pass_uvCoords;

layout (set = 0) uniform Uniforms
{
	mat4 ProjectionMatrix;
	mat4 ViewMatrix;
	mat4 ModelMatrix;
};

void main()
{
	mat4 projectionViewMatrix = ProjectionMatrix * ViewMatrix;
	vec4 worldPostition = ModelMatrix * vec4(in_position, 1.0);

	pass_colour = in_colour;
	pass_uvCoords = in_uvCoords;

	gl_Position = projectionViewMatrix * worldPostition;
}
