//GLSL code for fragment shader
#version 330 core

in vec2 texcoords;

out vec4 colour;

uniform sampler2D mytexture;

void main()
{
    colour = texture(mytexture, texcoords);
}