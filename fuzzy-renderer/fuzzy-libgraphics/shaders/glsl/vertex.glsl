#version 460 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;
layout (location = 3) in vec3 tangents;
layout (location = 4) in vec3 bitangents;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 world_vertex;
out vec3 world_normal;
out vec2 world_uv;
out vec3 world_tangent;
out vec3 world_bitangent;

void main()
{
    world_vertex = vec3(model * vec4(vertex, 1.0));
    world_normal = mat3(transpose(inverse(model))) * normal;
    world_tangent = mat3(transpose(inverse(model))) * tangents;
    world_bitangent = mat3(transpose(inverse(model))) * bitangents;
    world_uv = uv;

    gl_Position = projection * view * model * vec4(vertex, 1.0);
}
