#version 450 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;

struct ShaderOutput
{
	vec3 Colour;
	vec3 Normal;
	vec3 WorldPosition;
	vec3 CameraPosition;
	float IsSelected;
};

layout (location = 0) out ShaderOutput Output;

layout (set = 0) uniform Uniforms
{
	mat4 ProjectionMatrix;
	mat4 ViewMatrix;
};

layout(push_constant) uniform PushConstants
{
	mat4 ModelMatrix;
	vec3 Colour;
	float IsSelected;
} PieceConstants;

void main()
{
	mat4 projectionViewMatrix = ProjectionMatrix * ViewMatrix;
	vec4 worldPostition = PieceConstants.ModelMatrix * vec4(in_position, 1.0);

	Output.Colour = PieceConstants.Colour;
	Output.Normal = in_normal;
	Output.WorldPosition = worldPostition.xyz;
	Output.CameraPosition = (inverse(ViewMatrix) * vec4(0.0, 0.0, 1.0, 1.0)).xyz;
	Output.IsSelected = PieceConstants.IsSelected;

	gl_Position = projectionViewMatrix * worldPostition;
}
