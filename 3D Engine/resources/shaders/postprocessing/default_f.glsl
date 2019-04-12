#version 430 core

in vec2 TexCoords;

uniform sampler2D screen_tex;

out vec4 FragColor;

void main() {
    FragColor = texture(screen_tex, TexCoords);
}