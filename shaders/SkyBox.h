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
const char shader_skybox_vs[] = R"(
#version 330 core

layout (location = 0) in vec3 position;

out vec3 textureCoords;

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(position, 1.0);
	gl_Position = gl_Position.xyww;
	textureCoords = position;
}
)";

//FRAGMENT SHADER
const char shader_skybox_fs[] = R"(
#version 330 core

//Constants
const float gamma = 2.2;

//Uniforms
uniform samplerCube skyBoxTexture;

//Inputs
in vec3 textureCoords;

//Outputs
out vec4 diffuseColor;

void main()
{
	diffuseColor = texture(skyBoxTexture, textureCoords);

	//apply gamma correction
	diffuseColor.rgb = pow(diffuseColor.rgb, vec3(1.0 / gamma));
}
)";