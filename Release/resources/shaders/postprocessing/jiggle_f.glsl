#version 330 core

in vec2 TexCoords;

uniform sampler2D screenTexture;

uniform float time;

out vec4 FragColor;

void main()
{
	vec2 texc = TexCoords;
    texc.x += sin(texc.y * 4*2*3.14159 + time) / 100;
    FragColor = texture(screenTexture, texc);
}   