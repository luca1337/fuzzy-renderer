#version 460 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

// MVP
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 world_vertex;
out vec3 world_normal;
out vec2 world_uv;

void main()
{
	vec4 world_position		= model  * vec4(vertex, 1.0);
	world_position			= view * world_position;
	world_position			= projection * world_position;

	world_vertex			= vec3(model * vec4(vertex, 1.0));
	world_normal			= mat3(transpose(inverse(model))) * normal;
	world_uv				= uv;

	gl_Position				= world_position;
}