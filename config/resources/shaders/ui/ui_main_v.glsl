#version 430 core

layout(location = 0) in vec3 iPos;

void main() {
    gl_Position = vec4(iPos.x, iPos.y, 0.0f, 1.0f);
}