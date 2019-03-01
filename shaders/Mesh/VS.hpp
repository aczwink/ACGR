/*
 * Copyright (c) 2017-2019 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACGR.
 *
 * ACGR is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACGR is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACGR.  If not, see <http://www.gnu.org/licenses/>.
 */
const char shader_mesh_vs[] = R"(
#version 330 core

//Uniforms
uniform mat4 model;
uniform mat4 MVP;

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
)";