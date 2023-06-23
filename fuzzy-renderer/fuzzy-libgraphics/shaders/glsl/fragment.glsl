#version 460 core

out vec4 FragColor;

in vec3 world_vertex;
in vec3 world_normal;
in vec2 world_uv;
in vec3 world_tangent;
in vec3 world_bitangent;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normal;
    sampler2D height;
    sampler2D ambient;
    sampler2D emissive;
    sampler2D opacity;
    sampler2D displacement;
    sampler2D reflection;

    float shininess;
    float roughness;

    bool useTextures;
};

struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 eye;
uniform Material material;
uniform DirectionalLight dir_light;

vec3 calculateDirectionalLight(vec3 normal, vec3 viewDirection, DirectionalLight light, Material material)
{
    // Check if textures are used
    if (material.useTextures)
    {
        // Perform normal mapping
        vec3 normal_from_map = texture(material.normal, world_uv).xyz * 2.0 - 1.0; // Retrieve normal from normal map
        mat3 TBN = mat3(normalize(world_tangent), normalize(world_bitangent), normalize(world_normal)); // Create TBN matrix
        vec3 transformed_normal = normalize(TBN * normal_from_map); // Transform normal using TBN matrix

        vec4 diffuse_color = texture(material.diffuse, world_uv);
        vec4 specular_color = texture(material.specular, world_uv);

        float lambert_effect = max(dot(transformed_normal, -light.direction), 0.0);

        vec3 view_direction = normalize(eye - world_vertex);
        vec3 half_vector = normalize(-light.direction + view_direction);

        float specular_effect = pow(max(dot(transformed_normal, half_vector), 0.0), material.shininess);

        vec3 diffuse = diffuse_color.rgb * lambert_effect;
        vec3 specular = specular_color.rgb * specular_effect;

        // Calcolo del termine di rugosità
        float roughness_factor = 1.0 - material.shininess / 100.0;
        vec3 roughness_term = vec3(roughness_factor) * (1.0 - specular_color.rgb); // Modifica il valore di rugosità in base al fattore e al colore speculare

        vec3 final_color = light.ambient + light.diffuse * diffuse + light.specular * specular - material.roughness * roughness_term; // Sottrai il termine di rugosità per ridurre l'effetto lucido

        return final_color;
    }
    else
    {
        vec3 diffuse = light.diffuse;
        vec3 specular = light.specular;

        vec3 final_color = light.ambient + diffuse + specular - material.roughness * vec3(material.roughness);

        return final_color;
    }
}

void main()
{
    vec3 normalized_world_normal = normalize(world_normal);
    vec3 light_direction = normalize(eye - world_vertex);

    vec3 final_color = calculateDirectionalLight(normalized_world_normal, light_direction, dir_light, material);

    FragColor = vec4(final_color, 1.0);
}
