#version 430 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in mat3 TBN;

uniform vec3 cameraPos;

struct Material {
    sampler2D diffuse_map;
    sampler2D specular_map;
    float shininess;
};

layout(std140, binding = 2) uniform Camera {
    vec3 camera_position;
};

layout(location = 6) uniform Material material;
layout(location = 10) uniform sampler2D depth_map;
layout(location = 11) uniform sampler2D normal_map;
layout(location = 12) uniform samplerCube skybox;

void main()
{             
    vec3 norm = normalize(texture(normal_map, TexCoords).rgb * 2.0 - 1.0);
    norm = normalize(TBN * norm); 
    vec3 I = normalize(FragPos - camera_position);
    vec3 R = reflect(I, normalize(norm));
    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}