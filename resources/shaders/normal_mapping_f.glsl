#version 430 core

#define MAX_LIGHTS_PER_TYPE 15

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;

struct PointLight {
    vec3 ambient;        
    vec3 diffuse;  
    vec3 specular;
    vec3 position;
    float intensity;
};

struct DirectionalLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;
};

struct SpotLight {
    vec3 ambient;       // vec4
    vec3 diffuse;       // vec4
    vec3 specular;      // vec4
    vec3 position;      // vec4
    vec3 direction;     // vec3 + 1 float for the next variable
    float intensity;    // packed with with the previous vec3 to get 16 bytes
    float inner_angle;  // float, together with next float
    float outer_angle;  // float, 8 bytes padding added after this
};

layout(std140, binding = 1) uniform Lights {
    int point_light_count;
    int directional_light_count;
    int spot_light_count;
    PointLight point_lights[MAX_LIGHTS_PER_TYPE];
    DirectionalLight directional_lights[MAX_LIGHTS_PER_TYPE];
    SpotLight spot_lights[MAX_LIGHTS_PER_TYPE];
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
layout(location = 10) uniform sampler2D depth_map;
layout(location = 11) uniform sampler2D normal_map;

float saturate(float val) {
    if (val > 1.0) val = 1.0;
    return val;
}

vec3 saturate(vec3 val) {
    return vec3(saturate(val.x), saturate(val.y), saturate(val.z));
}

vec3 calc_point_light(PointLight light, vec3 norm, float shadow) {
    vec3 light_result = vec3(0.0f);
    // ambient lighting
    vec3 ambient = light.ambient * vec3(texture(material.diffuse_map, TexCoords));
    // diffuse lighting
    vec3 light_dir = normalize(light.position - FragPos);
    float diff = max(dot(light_dir, norm), 0.0);
    vec3 diffuse =  light.diffuse * (diff * vec3(texture(material.diffuse_map, TexCoords)));
    // specular lighting
    vec3 view_dir = normalize(camera_position - FragPos);
    vec3 halfway_dir = normalize(light_dir + view_dir); // blinn-phong halfway dir
    float spec = pow(max(dot(norm, halfway_dir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * vec3(texture(material.specular_map, TexCoords)));
    // apply light falloff
    float distance = length(light.position - FragPos);
    float falloff = light.intensity / (distance);
    light_result = (ambient + (1.0 - shadow) * (diffuse + specular)) * falloff;
    return saturate(light_result);
}

vec3 calc_directional_light(DirectionalLight light, vec3 norm, float shadow) {
    vec3 light_result = vec3(0.0f);
    // ambient lighting
    vec3 ambient = light.ambient * vec3(texture(material.diffuse_map, TexCoords));
    // diffuse lighting
    vec3 light_dir = normalize(-light.direction);
    float diff = max(dot(light_dir, norm), 0.0);
    vec3 diffuse =  light.diffuse * (diff * vec3(texture(material.diffuse_map, TexCoords)));
    // specular lighting
    vec3 view_dir = normalize(camera_position - FragPos);
    vec3 halfway_dir = normalize(light_dir + view_dir); // blinn_phong halfway dir
    float spec = pow(max(dot(norm, halfway_dir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular_map, TexCoords));
    
    light_result = ambient + (1.0 - shadow) * (diffuse + specular);

    return saturate(light_result);
}

vec3 calc_spot_light(SpotLight light, vec3 norm, float shadow) {
    // This lighting is the same as point lighting, 
    // except that smooth edges are added at the end for the spot light
    
    // ambient lighting
    vec3 ambient = vec3(light.ambient * vec3(texture(material.diffuse_map, TexCoords)));
    // diffuse lighting
    vec3 light_dir = normalize(light.position - FragPos);
    float diff = max(dot(light_dir, norm), 0.0);
    vec3 diffuse =  light.diffuse * (diff * vec3(texture(material.diffuse_map, TexCoords)));
    
    // specular lighting
    vec3 view_dir = normalize(camera_position - FragPos);
    vec3 halfway_dir = normalize(light_dir + view_dir); // blinn_phong halfway dir
    float spec = pow(max(dot(norm, halfway_dir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * vec3(texture(material.specular_map, TexCoords)));
    
    // spot light with smooth edges
    float theta = dot(light_dir, normalize(-light.direction)); 
    float epsilon = (light.inner_angle - light.outer_angle);
    float mod = clamp((theta - light.outer_angle) / epsilon, 0.0, 1.0);
    diffuse  *= mod;
    specular *= mod;

    // apply light falloff
    float distance = length(light.position - FragPos);
    float falloff = light.intensity / (distance); // don't square distance because of gamma correction
    
    return saturate((ambient + (1.0 - shadow) * (diffuse + specular)) * falloff);
}

float calc_shadow() {
    // perspective division
    vec3 projcoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    // values out of range will get no shadows
    if (projcoords.z > 1.0) return 0.0;

    // transform to from [-1, 1] to range [0, 1]
    projcoords = projcoords * 0.5 + 0.5;
    float closest_depth = texture(depth_map, projcoords.xy).r;
    float current_depth = projcoords.z;

    float bias = 0.005;
//    float shadow = current_depth - bias > closest_depth  ? 1.0 : 0.0;
    // PCF for soft shadows
    float shadow = 0.0;
    vec2 texel_size = 1.0 / textureSize(depth_map, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcf_depth = texture(depth_map, projcoords.xy + vec2(x, y) * texel_size).r; 
            shadow += current_depth - bias > pcf_depth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    return shadow;
}

void main() {
    vec3 norm = normalize(texture(normal_map, TexCoords).rgb * 2.0 - 1.0);
    vec3 light_result = vec3(0.0f);
    float shadow = calc_shadow();
    // Optimize: calculate certain vectors only once!
    for(int i = 0; i < point_light_count; ++i) {
        light_result += calc_point_light(point_lights[i], norm, shadow);
    }
    for(int i = 0; i < directional_light_count; ++i) {
        light_result += calc_directional_light(directional_lights[i], norm, shadow);
    }
    for(int i = 0; i < spot_light_count; ++i) {
        light_result += calc_spot_light(spot_lights[i], norm, shadow);
    }

    FragColor = vec4(light_result, 1.0);
}