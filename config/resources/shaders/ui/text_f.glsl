#version 430 core

in vec2 TexCoords;

layout(location = 5) uniform sampler2D glyph;
layout(location = 3) uniform vec3 text_color;

out vec4 FragColor;

void main() {
    vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(glyph, TexCoords).r);
    FragColor = sampled * vec4(text_color, 1.0f);
}