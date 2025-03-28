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

layout (std140, set = 1) readonly buffer Marking
{
	vec2 SelectedCell;
	vec4 MoveCells[21];
};

void main()
{
	vec3 lightPosition = (Input.CameraPosition + vec3(0.0, 10.0, 0.0)) * 1000.0;

	vec3 unitNormal = normalize(Input.Normal);
	vec3 lightVector = normalize(lightPosition - Input.WorldPosition);
	float NdotL = dot(unitNormal, lightVector);

	vec3 colour = Input.Colour;
	if (SelectedCell == Input.BoardPosition)
		colour += vec3(0.2, 0.1, 0.13);

	for (int i = 0;i < 21;i++)
	{
		if (MoveCells[i].xy == Input.BoardPosition || MoveCells[i].zw == Input.BoardPosition)
		{
			colour *= vec3(0.05, 0.02, 0.45);
			break;
		}
	}

	out_colour = vec4(colour * NdotL, 1.0);
}
