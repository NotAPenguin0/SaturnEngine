#version 430 core

layout(location = 4) uniform vec4 color;
layout(location = 5) uniform sampler2D tex;

in vec2 TexCoords;

out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    FragColor = (texture(tex, TexCoords)) * color;
}