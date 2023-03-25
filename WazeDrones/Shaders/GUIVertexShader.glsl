#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec4 aColor;
layout (location = 3) in int aIndex;
layout (location = 4) in uint SpecialEffects;

uniform uint worldTime;

out vec2 outTexture;
out vec4 color;
out int samplerHandle;

void main()
{
    vec3 pos = aPos;
    if((SpecialEffects & 0x1) == 1)
    {
        float speed = 2.0F;
        float pi = 3.14159265359F;
        float magnitude = (sin((aPos.y + aPos.x + worldTime * pi / ((28.0F) * speed))) * 0.15F + 0.15F) * 0.20F;
        float d0 = sin(worldTime * pi / (112.0F * speed)) * 3.0F - 1.5F;
        float d1 = sin(worldTime * pi / (142.0F * speed)) * 3.0F - 1.5F;
        float d2 = sin(worldTime * pi / (132.0F * speed)) * 3.0F - 1.5F;
        float d3 = sin(worldTime * pi / (122.0F * speed)) * 3.0F - 1.5F;
        pos.x += sin((worldTime * pi / (18.0F * speed)) + (-aPos.x + d0) * 1.6F + (aPos.z + d1) * 1.6F) * magnitude;
        pos.z += sin((worldTime * pi / (17.0F * speed)) + (aPos.z + d2) * 1.6F + (-aPos.x + d3) * 1.6F) * magnitude;
        pos.y += sin((worldTime * pi / (11.0F * speed)) + (aPos.z + d2) + (aPos.x + d3)) * (magnitude / 2.0F);
    }
	gl_Position = vec4(pos,1.0F);
	outTexture = aUV;
	color = aColor;
	samplerHandle = aIndex;
}