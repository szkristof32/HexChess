#version 450 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_colour;
layout (location = 2) in vec3 in_normal;
layout (location = 3) in vec2 in_boardPosition;

struct ShaderOutput
{
	vec3 Colour;
	vec3 Normal;
	vec3 WorldPosition;
	vec3 CameraPosition;
	vec2 BoardPosition;
};

layout (location = 0) out ShaderOutput Output;

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

	Output.Colour = in_colour;
	Output.Normal = in_normal;
	Output.WorldPosition = worldPostition.xyz;
	Output.CameraPosition = (inverse(ViewMatrix) * vec4(0.0, 0.0, 1.0, 1.0)).xyz;
	Output.BoardPosition = in_boardPosition;

	gl_Position = projectionViewMatrix * worldPostition;
}
