#version 460 core

layout (location = 0) in vec3 vertex;

out vec3 out_tex_coords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	out_tex_coords = vertex;
	gl_Position = projection * view * vec4(vertex, 1.0);
}