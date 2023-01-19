//GLSL code for vertex shader
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

out vec2 texcoords;

//rotation matrix
uniform mat4 rotationtransform;

void main()
{
	gl_Position = rotationtransform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	texcoords = aTex;
}