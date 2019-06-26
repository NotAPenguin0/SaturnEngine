#version 430 core

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec2 iTexCoords;

out vec2 TexCoords;

void main() {
    TexCoords = iTexCoords;
    gl_Position = vec4(iPos.x, iPos.y, 0.0f, 1.0f);
}