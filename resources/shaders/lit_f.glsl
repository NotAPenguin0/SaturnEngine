#version 430 core

#define MAX_LIGHTS_PER_TYPE 15

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

layout (location = 5) uniform sampler2D tex;

struct PointLight { //  base alignment  aligned offset
    vec3 ambient;   //  16              0                 
    vec3 diffuse;   //  16              16
    vec3 specular;  //  16              32

    vec3 position;  //  16              48
};

layout(std140, binding = 1) uniform Lights {
    int point_light_count;
    PointLight point_lights[MAX_LIGHTS_PER_TYPE];
};

layout(std140, binding = 2) uniform Camera {
    vec3 camera_position;
};

out vec4 FragColor;

void main() {
    vec3 norm = normalize(Normal);


    vec3 light_result = vec3(0.0f);
    for(int i = 0; i < point_light_count; ++i) {
        // ambient lighting
        light_result += point_lights[i].ambient;
        // diffuse lighting
        vec3 light_dir = normalize(point_lights[i].position - FragPos);
        float diff = max(dot(norm, light_dir), 0.0);
        vec3 diffuse = diff * point_lights[i].diffuse;
        light_result += diffuse;
        // specular lighting
        vec3 view_dir = normalize(camera_position - FragPos);
        vec3 reflect_dir = reflect(-light_dir, norm);
        float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
        vec3 specular = point_lights[i].specular * spec;
        light_result += specular;
    }

    FragColor = vec4(light_result * vec3(texture(tex, TexCoords)), 1.0);
}