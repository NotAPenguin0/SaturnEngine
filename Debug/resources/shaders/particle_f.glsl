#version 430 core

layout(location = 1) uniform vec4 color;
layout(location = 3) uniform sampler2D tex;

in vec2 TexCoords;

out vec4 FragColor;

void main() {
    FragColor = (texture(tex, TexCoords)) * color;
}