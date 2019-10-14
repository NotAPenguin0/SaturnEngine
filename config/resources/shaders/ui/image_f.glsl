#version 430 core

in vec2 TexCoords;

layout(location = 5) uniform sampler2D image;

out vec4 FragColor;

void main() {
    FragColor = vec4(vec3(texture(image, TexCoords)), 1.0);
}