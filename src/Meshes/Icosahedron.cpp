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
//Class header
#include <ACGR/Meshes/Primitives.hpp>
//Namespaces
using namespace ACGR;

constexpr float32 phi = (1 + sqrtf(5)) / 2;

static const Mesh::Vertex g_icosahedronVertices[] =
{
	{{-1, phi, 0}},
	{{1, phi, 0}},
	{{-1, -phi, 0}},
	{{1, -phi, 0}},

	{{0, -1, phi}},
	{{0, 1, phi}},
	{{0, -1, -phi}},
	{{0, 1, -phi}},

	{{phi, 0, -1}},
	{{phi, 0, 1}},
	{{-phi, 0, -1}},
	{{-phi, 0, 1}},
};

static const uint16 g_icosahedronIndices[] =
{
	0, 11, 5,
	0, 5, 1,
	0, 1, 7,
	0, 7, 10,
	0, 10, 11,

	1, 5, 9,
	5, 11, 4,
	11, 10, 2,
	10, 7, 6,
	7, 1, 8,

	3, 9, 4,
	3, 4, 2,
	3, 2, 6,
	3, 6, 8,
	3, 8, 9,

	4, 9, 5,
	2, 4, 11,
	6, 2, 10,
	8, 6, 7,
	9, 8, 1
};


//Constructor
Icosahedron::Icosahedron()
{
	this->SetVertices(12, g_icosahedronVertices);
	this->SetIndices(60, g_icosahedronIndices);

	this->ComputeBBox();
	this->ComputeNormals();
}