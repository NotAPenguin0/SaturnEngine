#version 430 core

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec2 unused1;
layout(location = 2) in vec3 unused2;

layout (std140, binding = 0) uniform Matrices {
    mat4 projection;
    mat4 view;
};

layout(location = 2) uniform mat4 model;

void main() {
    gl_Position = projection * view * model * vec4(iPos, 1.0);
}