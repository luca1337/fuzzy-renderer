#version 460 core

out vec4 FragColor;

in vec3 out_tex_coords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, out_tex_coords);
}