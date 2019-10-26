#version 430 core

out vec4 FragColor;

in vec3 TexCoords;

layout(location = 0) uniform samplerCube skybox;

void main() {
    FragColor = texture(skybox, TexCoords);
}