#version 430 core

layout(location = 0) in vec3 iPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Position;

void main() {
    Position = iPos;
    gl_Position = projection * view * model * vec4(iPos, 1.0);
}