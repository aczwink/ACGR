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
const char shader_normals_gs[] = R"(
#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 8) out;

//Constants
const float MAGNITUDE = 0.75f;

//Input
in VS_OUT
{
	vec3 normal;
} gs_in[];

//Outputs
out vec3 fragColor;


vec4 ComputeCenter()
{
	return (gl_in[0].gl_Position + gl_in[1].gl_Position + gl_in[2].gl_Position) / 3;
}

vec3 ComputeFaceNormal()
{
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);

	return normalize(cross(a, b));
}


void GenerateLine(int index)
{
	gl_Position = gl_in[index].gl_Position;
	EmitVertex();
	gl_Position = gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0f) * MAGNITUDE;
	EmitVertex();
	EndPrimitive();
}

void GenerateFaceNormal()
{
	vec3 normal;
	vec4 pos;

	pos = ComputeCenter();
	normal = ComputeFaceNormal();

	gl_Position = pos;
	EmitVertex();
	gl_Position = pos + vec4(normal, 0) * MAGNITUDE;
	EmitVertex();
	EndPrimitive();
}

void main()
{
	//vertex normals
	fragColor = vec3(1.0f, 1.0f, 0.0f);
	GenerateLine(0); // First vertex normal
	GenerateLine(1); // Second vertex normal
	GenerateLine(2); // Third vertex normal

	//face normals
	fragColor = vec3(0, 1, 1.0f);
	GenerateFaceNormal();
}
)";