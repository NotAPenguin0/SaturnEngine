#version 430 core

layout(location = 0) in vec3 iPos;

layout (std140, binding = 0) uniform Matrices {
    mat4 projection;
    mat4 view;
};

layout(location = 2) uniform mat4 model;

void main() {
    gl_Position = projection * view * model * vec4(iPos, 1.0);
}