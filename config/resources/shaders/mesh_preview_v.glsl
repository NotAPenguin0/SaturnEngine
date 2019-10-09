#version 430 core

// inputs

// attributes
layout(location = 0) in vec3 iPos;
layout(location = 2) in vec3 iNormal;

// uniforms
layout(location = 0) uniform mat4 PVM;
layout(location = 1) uniform mat4 Model;

// outputs
out vec3 Normal;
out vec3 FragPos;

void main() {
    Normal = normalize(mat3(transpose(inverse(Model))) * iNormal);
    FragPos = vec3(Model * vec4(iPos, 1.0));
    gl_Position = PVM * vec4(iPos, 1.0f);
}