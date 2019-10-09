#version 430 core

out vec4 FragColor;

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

layout(location = 2) uniform vec3 ObjColor;
layout(location = 3) uniform DirectionalLight light;
layout(location = 7) uniform vec3 CamPos;

in vec3 FragPos;
in vec3 Normal;


vec3 lighting() {
    vec3 light_result = vec3(0.0f);
    // ambient lighting
    vec3 ambient = light.ambient * ObjColor;
    // diffuse lighting
    vec3 light_dir = normalize(-light.direction);
    float diff = max(dot(light_dir, Normal), 0.0);
    vec3 diffuse =  light.diffuse * (diff * ObjColor);
    // specular lighting
    vec3 view_dir = normalize(CamPos - FragPos);
    vec3 halfway_dir = normalize(light_dir + view_dir); // blinn_phong halfway dir
    float spec = pow(max(dot(Normal, halfway_dir), 0.0), 32);
    vec3 specular = light.specular * spec * ObjColor;
    
    light_result = ambient + diffuse + specular;

    return light_result;
}

void main() {
    FragColor = vec4(lighting(), 1.0f);
}