#version 430 core

#define MAX_LIGHTS_PER_TYPE 15

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

struct PointLight {
    vec3 ambient;        
    vec3 diffuse;  
    vec3 specular;
    vec3 position;
};

struct DirectionalLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
};

layout(std140, binding = 1) uniform Lights {
    int point_light_count;
    int directional_light_count;
    PointLight point_lights[MAX_LIGHTS_PER_TYPE];
    DirectionalLight directional_lights[MAX_LIGHTS_PER_TYPE];
};

layout(std140, binding = 2) uniform Camera {
    vec3 camera_position;
};

out vec4 FragColor;

struct Material {
    sampler2D diffuse_map;
    sampler2D specular_map;
    float shininess;
};

layout(location = 6) uniform Material material;

vec3 calc_point_light(PointLight light, vec3 norm) {
    vec3 light_result = vec3(0.0f);
    // ambient lighting
    light_result += light.ambient * vec3(texture(material.diffuse_map, TexCoords));
    // diffuse lighting
    vec3 light_dir = normalize(light.position - FragPos);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse =  light.diffuse * (diff * vec3(texture(material.diffuse_map, TexCoords)));
    light_result += diffuse;
    // specular lighting
    vec3 view_dir = normalize(camera_position - FragPos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * vec3(texture(material.specular_map, TexCoords)));
    light_result += specular;

    return light_result;
}

vec3 calc_directional_light(DirectionalLight light, vec3 norm) {
    vec3 light_result = vec3(0.0f);
    // ambient lighting
    light_result += light.ambient * vec3(texture(material.diffuse_map, TexCoords));
    // diffuse lighting
    vec3 light_dir = normalize(-light.direction);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse =  light.diffuse * (diff * vec3(texture(material.diffuse_map, TexCoords)));
    light_result += diffuse;
    // specular lighting
    vec3 view_dir = normalize(camera_position - FragPos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular_map, TexCoords));
    
    light_result += specular;

    return light_result;
}

void main() {
    vec3 norm = normalize(Normal);
    vec3 light_result = vec3(0.0f);
    // Optimize: calculate certain vectors only once!
    for(int i = 0; i < point_light_count; ++i) {
        light_result += calc_point_light(point_lights[i], norm);
    }
    for(int i = 0; i < directional_light_count; ++i) {
        light_result += calc_directional_light(directional_lights[i], norm);
    }

    FragColor = vec4(light_result, 1.0);
}