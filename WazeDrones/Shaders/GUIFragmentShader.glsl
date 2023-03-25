#version 460 core
#extension GL_ARB_bindless_texture : enable

layout(bindless_sampler) uniform;

layout(binding = 0) uniform samplerBlock { uvec2 samplers[32]; };
in vec2 outTexture;
in vec4 color;
in flat int samplerHandle;

out vec4 FragColor;

void main()
{
    vec4 textureColor = texture(sampler2D(samplers[samplerHandle]) , outTexture);
    if(textureColor.a < 0.5)
        discard;
	FragColor = color*textureColor;
}
