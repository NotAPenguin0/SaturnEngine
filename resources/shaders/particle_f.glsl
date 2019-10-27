#version 430 core

// layout(location = 4) uniform vec3 color;
layout(location = 5) uniform sampler2D tex;

in vec2 TexCoords;
in vec4 color;


out vec4 FragColor;

void main() {
    vec4 sampled = texture(tex, TexCoords);
    if (sampled.a <= 0) { discard; }
    FragColor = sampled * color;
}