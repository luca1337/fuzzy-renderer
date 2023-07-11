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
    sampler2D albedo_map;
    sampler2D metallic_map;
    sampler2D normal_map;

    float metallic;
    float roughness;
    float occlusion_strength;
    float emission_strength;

    vec3 albedo_color;
    vec3 emission_color;

    bool use_textures;
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
    int isActive;
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
    float dispersionFactor = 1.0 - material.roughness;

    float metallic = max(material.metallic, 0.01f);
    float intensity = 1.0 - material.roughness;  // Intensità della riflessione speculare

    return pow(max(dot(viewDir, reflectDir), 0.0), (metallic * 256)) * dispersionFactor * intensity;
}


vec3 calculateNormal(vec3 worldNormal, vec3 worldTangent, vec3 worldBitangent, vec3 textureNormal) 
{
    vec3 normal = textureNormal * 2.0 - 1.0;
    vec3 tangent = normalize(worldTangent);
    vec3 bitangent = normalize(worldBitangent);
    mat3 TBN = mat3(tangent, bitangent, worldNormal);
    return normalize(TBN * normal);
}

void calculateNormalAndTextures(inout vec3 normal, inout vec3 diffuseTexture, inout vec3 specularTexture)
{
    if (material.use_textures) 
    {
        vec3 normal_tex = texture(material.normal_map, world_uv).rgb;
        normal = calculateNormal(world_normal, world_tangent, world_bitangent, normal_tex);
        diffuseTexture = texture(material.albedo_map, world_uv).rgb;
        specularTexture = texture(material.metallic_map, world_uv).rgb;
    } 
    else 
    {
        normal = normalize(world_normal);
        diffuseTexture = material.albedo_color;
        specularTexture = material.albedo_color;
    }
}

LightingResult calculateDirectionalLight(Light light, float specularStrength, vec3 diffuseTexture, vec3 normalTexture, vec3 specularTexture) 
{
    vec3 ambient = light.intensity * light.color.rgb;

    vec3 normal = normalTexture;
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
    vec3 adjustedNormal = normalTexture;
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
    vec3 surfaceNormal = normalTexture;
    
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
    vec3 ambient = ambientStrength * diffuseTexture;

    LightingResult totalRes;
    totalRes.diffuseColor = vec3(0.0);
    totalRes.specularColor = vec3(0.0);

    for (int lightIdx = 0; lightIdx < MAX_LIGHTS; lightIdx++)
    {
        LightingResult res;

        Light currentLight = lights[lightIdx];

        if (currentLight.isActive == 0)
            continue;

        if (currentLight.type == 0)
        {
            res = calculateDirectionalLight(currentLight, specularStrength, diffuseTexture, normalTexture, specularTexture);
        }
        else if (currentLight.type == 1)
        {
            res = calculatePointLight(currentLight, specularStrength, diffuseTexture, normalTexture, specularTexture);
        }
        else if (currentLight.type == 2)
        {
            res = calculateSpotLight(currentLight, specularStrength, diffuseTexture, normalTexture, specularTexture, 50.2, 73.4);
        }

        totalRes.diffuseColor += res.diffuseColor;
        totalRes.specularColor += res.specularColor;
    }

    vec3 result = ambient + totalRes.diffuseColor + totalRes.specularColor;
    return result;
}

void main()
{
    float ambientStrength = 0.25;
    float specularStrength = 0.9;

    vec3 normalTexture;
    vec3 diffuseTexture;
    vec3 specularTexture;

    calculateNormalAndTextures(normalTexture, diffuseTexture, specularTexture);

    vec3 lighting = calculateLighting(ambientStrength, specularStrength, diffuseTexture, normalTexture, specularTexture);

    FragColor = vec4(lighting, 1.0);
}