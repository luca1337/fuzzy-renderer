#version 460 core

out vec4 FragColor;

in vec3 world_vertex;
in vec3 world_normal;
in vec2 world_uv;
in vec3 world_tangent;
in vec3 world_bitangent;

struct LightingResult {
    vec3 diffuseColor;
    vec3 specularColor;
};

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

uniform vec3 eye;
uniform Material material;
uniform vec3 global_ambient_color;

#define MAX_LIGHTS 256

struct Light {
    vec3 position;
    vec3 direction;
    float intensity;
    vec3 attenuation;
    vec4 color;
    int type;
    int is_active;
};

layout(std140) uniform LightsBlock {
    Light lights[MAX_LIGHTS];
};

float calculateDiffuse(vec3 lightDir, vec3 normal) 
{
    return max(dot(lightDir, normal), 0.0);
}

float calculateSpecular(vec3 lightDir, vec3 viewDir, vec3 normal) 
{
    vec3 reflectDir = reflect(-lightDir, normal);
    
    // Calcolo del fattore di dispersione basato sulla rugosità
    float dispersionFactor = 1.0 - material.roughness;
    
    // Modifica del calcolo del colore speculare con il fattore di dispersione
    return pow(max(dot(viewDir, reflectDir), 0.0), material.shininess) * dispersionFactor;
}

vec3 calculateNormal(vec3 worldNormal, vec3 worldTangent, vec3 worldBitangent, vec3 textureNormal) 
{
    vec3 normal = textureNormal * 2.0 - 1.0;
    vec3 tangent = normalize(worldTangent);
    vec3 bitangent = normalize(worldBitangent);
    mat3 TBN = mat3(tangent, bitangent, worldNormal);
    return normalize(TBN * normal);
}

LightingResult calculateDirectionalLight(Light light, float specularStrength, vec3 diffuseTexture, vec3 normalTexture, vec3 specularTexture) 
{
    vec3 ambient = light.intensity * light.color.rgb;

    vec3 normal = calculateNormal(world_normal, world_tangent, world_bitangent, normalTexture);
    vec3 lightDirection = normalize(light.direction);

    float diffuse = calculateDiffuse(lightDirection, normal);
    vec3 diffuseColor = diffuse * diffuseTexture * light.color.rgb * light.intensity;

    vec3 viewDir = normalize(eye - world_vertex);
    float specular = calculateSpecular(lightDirection, viewDir, normal);
    vec3 specularColor = specularStrength * specular * specularTexture * light.color.rgb * light.intensity;

    LightingResult result;
    result.diffuseColor = diffuseColor;
    result.specularColor = specularColor;
    return result;
}

LightingResult calculatePointLight(Light light, float specularStrength, vec3 diffuseTexture, vec3 normalTexture, vec3 specularTexture)
{
    vec3 lightDir = normalize(light.position - world_vertex);
    float dist = length(light.position - world_vertex);
    float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * dist + light.attenuation.z * dist * dist);
    vec3 adjustedNormal = calculateNormal(world_normal, world_tangent, world_bitangent, normalTexture);
    float diffuse = calculateDiffuse(lightDir, adjustedNormal);
    vec3 diffuseColor = diffuse * diffuseTexture * light.color.rgb * light.intensity * attenuation;
    vec3 reflectDir = reflect(-lightDir, adjustedNormal);
    vec3 viewDir = normalize(eye - world_vertex);
    float specular = calculateSpecular(lightDir, viewDir, adjustedNormal);
    vec3 specularColor = specularStrength * specular * specularTexture * light.color.rgb * light.intensity * attenuation;

    LightingResult result;
    result.diffuseColor = diffuseColor;
    result.specularColor = specularColor;
    return result;
}

LightingResult calculateSpotLight(Light light, float specularStrength, vec3 diffuseTexture, vec3 normalTexture, vec3 specularTexture, float cutoffAngle, float outerCutoffAngle)
{
    vec3 lightDirection = normalize(light.position - world_vertex);
    float dist = length(light.position - world_vertex);
    float attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * dist + light.attenuation.z * (dist * dist));
    
    vec3 viewDirection = normalize(eye - world_vertex);
    vec3 surfaceNormal = calculateNormal(world_normal, world_tangent, world_bitangent, normalTexture);
    
    float diff = calculateDiffuse(lightDirection, surfaceNormal);
    vec3 diffuse = diff * diffuseTexture * light.color.rgb * light.intensity * attenuation;
    
    float spec = calculateSpecular(lightDirection, viewDirection, surfaceNormal);
    vec3 specular = specularStrength * spec * specularTexture * light.color.rgb * light.intensity * attenuation;
    
    float theta = dot(lightDirection, normalize(-light.direction));
    float cutoff = cos(radians(cutoffAngle));
    float outerCutoff = cos(radians(outerCutoffAngle));
    float spotlightEffect = smoothstep(outerCutoff, cutoff, theta);
    
    LightingResult result;
    result.diffuseColor = diffuse * spotlightEffect;
    result.specularColor = specular * spotlightEffect;
    return result;
}

vec3 calculateLighting(float ambientStrength, float specularStrength, vec3 diffuseTexture, vec3 normalTexture, vec3 specularTexture)
{
    vec3 ambient = ambientStrength * vec3(1);

    LightingResult totalRes;
    totalRes.diffuseColor = vec3(0.0);
    totalRes.specularColor = vec3(0.0);

    for (int light_idx = 0; light_idx != MAX_LIGHTS; light_idx++)
    {
        LightingResult res;

        Light current_light = lights[light_idx];

        if (current_light.is_active == 0)
            continue;

        if (current_light.type == 0)
        {
            res = calculateDirectionalLight(current_light, specularStrength, diffuseTexture, normalTexture, specularTexture);
        }
        if (current_light.type == 1)
        {
            res = calculatePointLight(current_light, specularStrength, diffuseTexture, normalTexture, specularTexture);
        }
        if (current_light.type == 2)
        {
            res = calculateSpotLight(current_light, specularStrength, diffuseTexture, normalTexture, specularTexture, 50.2, 73.4);
        }

        totalRes.diffuseColor += res.diffuseColor;
        totalRes.specularColor += res.specularColor;
    }

    vec3 result = ambient + totalRes.diffuseColor + totalRes.specularColor;
    return result;
}

void main()
{
    float ambientStrength = 0.1;
    float specularStrength = 0.9;

    vec3 diffuseTexture = texture(material.diffuse, world_uv).rgb;
    vec3 specularTexture = texture(material.specular, world_uv).rgb;
    vec3 normalTexture = texture(material.normal, world_uv).rgb;

    vec3 lighting = calculateLighting(ambientStrength, specularStrength, diffuseTexture, normalTexture, specularTexture);

    FragColor = vec4(lighting, 1.0);
}