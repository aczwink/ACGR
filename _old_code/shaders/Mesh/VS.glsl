#version 330 core
#extension GL_ARB_explicit_uniform_location : require

//Uniforms
layout (location = 0) uniform mat4 model;
layout (location = 1) uniform mat4 MVP;

//Inputs
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexTexCoords;

//Output
out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoords;

void main()
{
	mat3 normalMatrix;
	
	normalMatrix = mat3(transpose(inverse(model))); //TODO: do this on the cpu, when inverse is defnied there ;)
	
	gl_Position = MVP * vec4(vertexPosition, 1);
	
	//pass to fragment shader
	fragPos = vec3(model * vec4(vertexPosition, 1));
	fragNormal = normalMatrix * vertexNormal;
	fragTexCoords = vec2(vertexTexCoords.x, 1 - vertexTexCoords.y); //reference point for textures in opengl is left bottom
}