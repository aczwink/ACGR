#version 330 core

//Uniforms
uniform sampler2D screenTexture;

//Inputs
in vec2 UV;

//Output
out vec4 color;

void main()
{
	color = texture(screenTexture, UV);
}