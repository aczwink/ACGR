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
//Vertex Shader
const char shader_shadow_vs[] = R"(
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
)";

//FRAGMENT SHADER
const char shader_shadow_fs[] = R"(
#version 330 core

void main()
{
	//nothing to be done here
}
)";