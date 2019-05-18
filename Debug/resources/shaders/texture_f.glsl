#version 430 core

in vec2 TexCoords;

layout (location = 3) uniform sampler2D tex;

out vec4 FragColor;

void main() {
    FragColor = texture(tex, TexCoords);
}