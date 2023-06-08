#version 460 core

out vec4 FragColog;

in vec3 world_vertex;
in vec3 world_normal;
in vec2 world_uv;

// Light
uniform vec3 eye;

// texture
uniform sampler2D albedo;

void main()
{
	vec3 normalized_world_normal = normalize(world_normal);
	vec3 light_direction = normalize(eye - world_vertex);

	vec3 texel = texture(albedo, world_uv).xyz;

	float lambert_effect = max(dot(normalized_world_normal, light_direction), 0.0);

	vec3 final_color = vec3(1.0, 1.0, 1.0) * lambert_effect;

	FragColog = vec4(final_color, 1);
}