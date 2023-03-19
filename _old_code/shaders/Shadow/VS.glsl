#version 330 core
#extension GL_ARB_explicit_uniform_location : require

//Uniforms
layout (location = 0) uniform mat4 lightMatrix;
layout (location = 1) uniform mat4 model;

//Inputs
layout (location = 0) in vec3 position;

void main()
{
	gl_Position = lightMatrix * model * vec4(position, 1);
}