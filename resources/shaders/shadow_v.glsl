#version 430 core

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec2 iTexCoords;
layout(location = 2) in vec3 iNormal;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;
out vec4 FragPosLightSpace;

layout (std140, binding = 0) uniform Matrices {
    mat4 projection;
    mat4 view;
};

layout(location = 2) uniform mat4 model;
layout(location = 9) uniform mat4 lightspace_matrix;

void main() {
    TexCoords = iTexCoords;
    // calculate this matrix on cpu for efficiency later
    Normal = mat3(transpose(inverse(model))) * iNormal;
    FragPos = vec3(model * vec4(iPos, 1.0));
    FragPosLightSpace = lightspace_matrix * vec4(FragPos, 1.0);
    gl_Position = projection * view * model * vec4(iPos, 1.0);
}