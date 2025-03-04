#version 450 core

layout (location = 0) in vec3 pass_colour;
layout (location = 1) in vec2 pass_uvCoords;

layout (location = 0) out vec4 out_colour;

layout (set = 1) uniform sampler2D TextureSampler;

void main()
{
	out_colour = texture(TextureSampler, pass_uvCoords);
}
