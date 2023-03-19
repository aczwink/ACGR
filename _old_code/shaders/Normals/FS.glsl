#version 330 core

//Inputs
in vec3 fragColor;

//Outputs
out vec4 color;

void main()
{
	color = vec4(fragColor, 1.0f);
}