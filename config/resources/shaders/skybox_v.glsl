#version 430 core

layout(location = 0) in vec3 iPos;

layout (std140, binding = 0) uniform Matrices {
    mat4 projection;
    mat4 view;
};

out vec3 TexCoords;

void main() {
    TexCoords = iPos;
    mat4 new_view = mat4(mat3(view));
    vec4 pos = projection * new_view * vec4(iPos, 1.0);
    gl_Position = pos.xyww;
}