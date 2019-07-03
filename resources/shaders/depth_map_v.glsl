#version 430 core

layout(location = 0) in vec3 iPos;

layout(location = 9) uniform mat4 lightspace_matrix;
layout(location = 2) uniform mat4 model;

void main() {
    gl_Position = lightspace_matrix * model * vec4(iPos, 1.0);
}