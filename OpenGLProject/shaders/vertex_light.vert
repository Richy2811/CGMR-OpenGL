//GLSL code for light source vertex shader
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

out vec2 texcoords;

//rotation matrices
uniform mat4 model;			//local model transformations
uniform mat4 view;			//view transformation
uniform mat4 projection;	//projection transformation

void main()
{
	gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	texcoords = aTex;
}