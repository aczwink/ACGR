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

static const Mesh::Vertex g_planePositions[] =
{
	//left
	{{-0.5f, 0.5f, 0}, {0, 0, 1}, {0, 1}}, //top
	{{-0.5f, -0.5f, 0}, {0, 0, 1}, {0, 0}}, //bottom

	//right
	{{0.5f, 0.5f, 0}, {0, 0, 1}, {1, 1}}, //top
	{{0.5f, -0.5f, 0}, {0, 0, 1}, {1, 0}} //bottom
};

static const uint16 g_planeIndices[] =
{
	0, 1, 3, //lower
	0, 3, 2, //upper
};

//Constructor
Plane::Plane()
{
	this->SetIndices(6, g_planeIndices);
	this->SetVertices(4, g_planePositions);

	this->ComputeBBox();
}