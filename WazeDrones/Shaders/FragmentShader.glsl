#version 460 core
#extension GL_ARB_bindless_texture : enable

layout(bindless_sampler) uniform;

uniform samplerBlock { uvec2 samplers[32]; };
in vec2 outTexture;
in vec4 color;
in flat int samplerHandle;

out vec4 FragColor;

void main()
{
	FragColor = color*texture(sampler2D(samplers[samplerHandle]) , outTexture);
}