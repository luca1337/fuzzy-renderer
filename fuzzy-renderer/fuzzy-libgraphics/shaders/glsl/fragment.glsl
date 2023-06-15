#version 460 core

out vec4 FragColor;

in vec3 world_vertex;
in vec3 world_normal;
in vec2 world_uv;
in vec3 world_tangent;
in vec3 world_bitangent;

uniform vec3 eye;
uniform sampler2D texture_diffuse0;
uniform sampler2D texture_specular0;
uniform sampler2D texture_normal0;

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

    // Perform normal mapping
    vec3 normal_from_map = texture(texture_normal0, world_uv).xyz * 2.0 - 1.0; // Retrieve normal from normal map
    mat3 TBN = mat3(normalize(world_tangent), normalize(world_bitangent), normalized_world_normal); // Create TBN matrix
    vec3 transformed_normal = normalize(TBN * normal_from_map); // Transform normal using TBN matrix

    vec4 diffuse_color = texture(texture_diffuse0, world_uv);
    vec4 specular_color = texture(texture_specular0, world_uv);

    vec3 ambient_color = vec3(0.3, 0.3, 0.3);
    float shininess = 32.0;
    float roughness = 0.3; // Valore di rugosità (0.0 per superficie liscia, 1.0 per superficie molto rugosa)

    float lambert_effect = max(dot(transformed_normal, light_direction), 0.0);

    vec3 view_direction = normalize(eye - world_vertex);
    vec3 reflect_direction = reflect(-light_direction, transformed_normal);

    float specular_effect = pow(max(dot(view_direction, reflect_direction), 0.0), shininess);

    vec3 ambient = ambient_color * diffuse_color.rgb;
    vec3 diffuse = diffuse_color.rgb * lambert_effect;
    vec3 specular = specular_color.rgb * specular_effect;

    // Configurazione specifica per tessuti
    ambient *= 0.8; // Riduci l'intensità dell'ambient per ridurre l'effetto "plasticoso"
    diffuse *= 1.2; // Aumenta l'intensità della componente diffusa per una maggiore visibilità dei dettagli
    specular *= 0.8; // Riduci l'intensità della componente speculare per una resa meno lucida

    // Calcolo del termine di rugosità
    float roughness_factor = 1.0 - shininess / 100.0;
    vec3 roughness_term = vec3(roughness_factor) * (1.0 - specular_color.rgb); // Modifica il valore di rugosità in base al fattore e al colore speculare

    vec3 final_color = ambient + diffuse + specular - roughness * roughness_term; // Sottrai il termine di rugosità per ridurre l'effetto lucido

    FragColor = vec4(final_color, 1.0);
}
