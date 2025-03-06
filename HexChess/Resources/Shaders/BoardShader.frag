#version 450 core

struct ShaderOutput
{
	vec3 Colour;
	vec3 Normal;
	vec3 WorldPosition;
	vec3 CameraPosition;
};

layout (location = 0) in ShaderOutput Input;

layout (location = 0) out vec4 out_colour;

void main()
{
	vec3 unitNormal = normalize(Input.Normal);
	vec3 lightVector = normalize(Input.CameraPosition - Input.WorldPosition);
	float NdotL = dot(unitNormal, lightVector);

	out_colour = vec4(Input.Colour * NdotL, 1.0);
}
