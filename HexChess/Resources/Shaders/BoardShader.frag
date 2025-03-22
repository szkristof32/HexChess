#version 450 core

struct ShaderOutput
{
	vec3 Colour;
	vec3 Normal;
	vec3 WorldPosition;
	vec3 CameraPosition;
	vec2 BoardPosition;
};

layout (location = 0) in ShaderOutput Input;

layout (location = 0) out vec4 out_colour;

layout (set = 1) uniform Colouring
{
	vec2 ExtraColouredCell;
};

void main()
{
	vec3 lightPosition = (Input.CameraPosition + vec3(0.0, 10.0, 0.0)) * 1000.0;

	vec3 unitNormal = normalize(Input.Normal);
	vec3 lightVector = normalize(lightPosition - Input.WorldPosition);
	float NdotL = dot(unitNormal, lightVector);

	vec3 colour = Input.Colour;
	if (ExtraColouredCell == Input.BoardPosition)
		colour += vec3(0.2, 0.1, 0.13);

	out_colour = vec4(colour * NdotL, 1.0);
}
