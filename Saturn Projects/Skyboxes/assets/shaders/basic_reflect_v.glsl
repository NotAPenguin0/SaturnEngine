#version 430 core

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec2 iTexCoords;
layout(location = 2) in vec3 iNormal;
layout(location = 3) in vec3 iTangent;

out vec2 TexCoords;
out vec3 FragPos;
out vec4 FragPosLightSpace;
out mat3 TBN;
out vec3 Normal;

layout (std140, binding = 0) uniform Matrices {
    mat4 projection;
    mat4 view;
};

layout(location = 2) uniform mat4 model;
layout(location = 9) uniform mat4 lightspace_matrix;

void main() {
    vec3 T = normalize(vec3(model * vec4(iTangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(iNormal, 0.0)));
    vec3 B = cross(N, T);
    TBN = mat3(T, B, N);

    Normal = iNormal;

    TexCoords = iTexCoords;
    FragPos = vec3(model * vec4(iPos, 1.0));

    FragPosLightSpace = lightspace_matrix * vec4(FragPos, 1.0);
    gl_Position = projection * view * model * vec4(iPos, 1.0);
}