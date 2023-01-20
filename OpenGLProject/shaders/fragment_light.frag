//GLSL code for light source fragment shader
#version 330 core

in vec2 texcoords;

out vec4 colour;

uniform sampler2D mylighttexture;

void main()
{
    colour = texture(mylighttexture, texcoords);
}