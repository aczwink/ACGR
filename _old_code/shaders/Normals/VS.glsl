#version 330 core
#extension GL_ARB_explicit_uniform_location : require

//Uniforms
layout (location = 0) uniform mat4 model;
layout (location = 1) uniform mat4 MVP;
uniform mat4 view; //TODO: location?
uniform mat4 projection; //TODO: location?

//Inputs
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;

//Output
out VS_OUT
{
	vec3 normal;
} vs_out;

void main()
{
	mat3 normalMatrix;
	
	normalMatrix = mat3(transpose(inverse(view * model)));
	
	gl_Position = MVP * vec4(vertexPosition, 1);
	vs_out.normal = normalize(vec3(projection * vec4(normalMatrix * vertexNormal, 1.0)));
}