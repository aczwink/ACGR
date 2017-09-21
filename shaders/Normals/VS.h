/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
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
const char shader_normals_vs[] = R"(
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
)";