#version 330 core

//Constants
const float gamma = 2.2;

//Uniforms
uniform samplerCube skyBoxTexture;

//Inputs
in vec3 textureCoords;

//Outputs
out vec4 color;

void main()
{
	color = texture(skyBoxTexture, textureCoords);
	
	//apply gamma correction
	color.rgb = pow(color.rgb, vec3(1.0 / gamma));
}