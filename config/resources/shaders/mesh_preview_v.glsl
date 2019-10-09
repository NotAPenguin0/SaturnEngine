#version 430 core

layout(location = 0) in vec3 iPos;

layout(location = 0) uniform mat4 PVM;

void main() {
    gl_Position = PVM * vec4(iPos, 1.0f);
}