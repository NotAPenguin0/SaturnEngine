#version 430 core

layout(location = 5) uniform sampler2D screenTexture;

in vec2 TexCoords;

out vec4 FragColor;

void  main()
{
	FragColor = texture(screenTexture, TexCoords);
}