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
const char shader_frame_vs[] = R"(
#version 330 core

//Inputs
layout (location = 0) in vec2 position;

//Output
out vec2 UV;

void main()
{
    gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);
    UV = vec2((position.x + 1) / 2, (position.y + 1) / 2);
}
)";

//FRAGMENT SHADER
const char shader_frame_fs[] = R"(
#version 330 core

//Uniforms
uniform sampler2D screenTexture;

//Inputs
in vec2 UV;

//Output
out vec4 diffuseColor;

void main()
{
	diffuseColor = texture(screenTexture, UV);
}
)";