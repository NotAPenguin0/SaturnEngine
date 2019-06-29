#version 430 core

#define MAX_LIGHTS_PER_TYPE 15

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

layout (location = 5) uniform sampler2D tex;

struct PointLight {
    vec3 ambient;        
    vec3 diffuse;  
    vec3 specular;
    vec3 position;
};

layout(std140, binding = 1) uniform Lights {
    int point_light_count;
    PointLight point_lights[MAX_LIGHTS_PER_TYPE];
};

layout(std140, binding = 2) uniform Camera {
    vec3 camera_position;
};

out vec4 FragColor;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float shininess;
};

layout(location = 6) uniform Material material;

void main() {
    vec3 norm = normalize(Normal);

    vec3 light_result = vec3(0.0f);
    for(int i = 0; i < point_light_count; ++i) {
        // ambient lighting
        light_result += point_lights[i].ambient * material.ambient;
        // diffuse lighting
        vec3 light_dir = normalize(point_lights[i].position - FragPos);
        float diff = max(dot(norm, light_dir), 0.0);
        vec3 diffuse =  point_lights[i].diffuse * (diff * material.diffuse);
        light_result += diffuse;
        // specular lighting
        vec3 view_dir = normalize(camera_position - FragPos);
        vec3 reflect_dir = reflect(-light_dir, norm);
        float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
        vec3 specular = point_lights[i].specular * (spec * material.specular);
        light_result += specular;
    }

    FragColor = vec4(light_result * vec3(texture(tex, TexCoords)), 1.0);
}