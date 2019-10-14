#version 430 core

layout(location = 5) uniform sampler2D screenTexture;

layout(location = 2) uniform float ui_opacity;

in vec2 TexCoords;

out vec4 FragColor;

void main()
{
	FragColor = vec4(vec3(texture(screenTexture, TexCoords)), ui_opacity);
}