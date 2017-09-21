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
#include <ACGR/Meshes/Procedural.hpp>
//Local
#include <ACGR/Meshes/Primitives.hpp>
//Namespaces
using namespace ACGR;

//Constructor
IcoSphere::IcoSphere(uint8 nSubdivisions)
{
	Icosahedron icosahedron;

	this->SetVertices(icosahedron.GetNumberOfVertices(), icosahedron.GetVertices());
	this->SetIndices(icosahedron.GetNumberOfIndices(), icosahedron.GetIndices16());

	for(uint8 i = 0; i < nSubdivisions; i++)
		this->Subdivide();

	//normalize vertices so that they lie on the unit sphere
	for(uint32 i = 0; i < this->GetNumberOfVertices(); i++)
		this->GetVertices()[i].position = this->GetVertices()[i].position.Normalize();

	this->ComputeBBox();
}