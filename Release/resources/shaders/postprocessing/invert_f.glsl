#version 330 core

uniform sampler2D screenTexture;

in vec2 TexCoords;

out vec4 FragColor;


void main()
{
    FragColor = vec4(vec3(1.0 - texture(screenTexture, TexCoords)), 1.0);
}  