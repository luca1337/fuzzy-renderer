#version 460 core

out vec4 FragColor;

in vec3 world_vertex;
in vec3 world_normal;
in vec2 world_uv;

// Light
uniform vec3 eye;

// texture
uniform sampler2D albedo;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

void main()
{
    vec3 normalized_world_normal = normalize(world_normal);
    vec3 light_direction = normalize(eye - world_vertex);
    
    vec3 texel = vec3(1.0); // Colore di base dell'oggetto
    
    vec3 ambient_color = vec3(0.2, 0.2, 0.2); // Colore dell'illuminazione ambientale
    vec3 diffuse_color = vec3(0.5, 0.5, 0.5); // Colore del componente diffuso
    vec3 specular_color = vec3(1.0, 1.0, 1.0); // Colore del componente speculare
    float shininess = 32.0; // Fattore di brillantezza
    
    float lambert_effect = max(dot(normalized_world_normal, light_direction), 0.0);
    
    vec3 view_direction = normalize(eye - world_vertex);
    vec3 reflect_direction = reflect(-light_direction, normalized_world_normal);
    
    float specular_effect = pow(max(dot(view_direction, reflect_direction), 0.0), shininess);
    
    vec3 ambient = ambient_color * texel;
    vec3 diffuse = diffuse_color * lambert_effect * texel;
    vec3 specular = specular_color * specular_effect;
    
    vec3 final_color = ambient + diffuse + specular;
    
    FragColor = vec4(final_color, 1.0);
}
