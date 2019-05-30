#version 430 core

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec2 iTexCoords;

layout(std140, binding = 0) uniform Matrices {
    mat4 projection;
    mat4 view;
};

out vec2 TexCoords;

layout(location = 2) uniform vec3 position;
layout(location = 3) uniform vec3 scale;

void main() {   
    TexCoords = iTexCoords;

    vec3 camright_world = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 camup_world = vec3(view[0][1], view[1][1], view[2][1]);
//    gl_Position = projection * view * model * vec4(iPos, 1.0);
    vec3 worldspace = position 
    + camright_world * iPos.x * scale.x 
    + camup_world * iPos.y * scale.y;
    gl_Position = projection * view * vec4(worldspace, 1.0);
}