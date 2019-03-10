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
//Class Header
#include <ACGR/Mesh.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Math;
using namespace ACGR;

//Local functions
static uint16 SubdivEdge(Mesh::Vertex *vertices, uint16 i0, uint16 i1, uint16 &currentVertexIndex, Map<uint64, uint16> &edgeIndexMap)
{
	auto it = edgeIndexMap.Find(Unsigned<uint64>::Concat(i0, i1));
	if(it != edgeIndexMap.end())
		return (*it).value;

	auto it2 = edgeIndexMap.Find(Unsigned<uint64>::Concat(i1, i0));
	if(it2 != edgeIndexMap.end())
		return (*it2).value;

	Mesh::Vertex &v0 = vertices[i0];
	Mesh::Vertex &v1 = vertices[i1];

	//add vertex
	edgeIndexMap[Unsigned<uint64>::Concat(i0, i1)] = currentVertexIndex;
	Mesh::Vertex &v_new = vertices[currentVertexIndex];

	v_new.position = 0.5f * (v0.position + v1.position);
	v_new.normal = 0.5f * (v0.normal + v1.normal);
	v_new.textureCoords = 0.5f * (v0.textureCoords + v1.textureCoords);

	return currentVertexIndex++;
}

//Constructor
Mesh::Mesh()
{
	this->nIndices = 0;
	this->nVertices = 0;
	this->pIndices16 = nullptr;
	//this->pIndices32 = nullptr;
	this->pVertices = nullptr;
	this->pMaterial = nullptr;
}

//Destructor
Mesh::~Mesh()
{
	if(this->pVertices)
		MemFree(this->pVertices);
	if(this->pIndices16)
		MemFree(this->pIndices16);
}

//Public methods
void Mesh::SetIndices(uint16 nIndices, const uint16 *pIndices)
{
	this->nIndices = nIndices;

	if(this->pIndices16)
		MemFree(this->pIndices16);

	this->pIndices16 = (uint16 *)MemAlloc(nIndices * sizeof(*pIndices));
	MemCopy(this->pIndices16, pIndices, nIndices * sizeof(*pIndices));
}

void Mesh::SetVertices(uint32 nVertices, const Vertex *pVertices)
{
	this->nVertices = nVertices;

	if(this->pVertices)
		MemFree(this->pVertices);

	this->pVertices = (Vertex *)MemAlloc(nVertices * sizeof(*pVertices));
	MemCopy(this->pVertices, pVertices, nVertices * sizeof(*pVertices));
}

//Protected methods
void Mesh::ComputeBBox()
{
	this->bbox = AxisAlignedBoxS();

	for(uint32 i = 0; i < nVertices; i++)
		this->bbox.Merge(pVertices[i].position);

	//check for empty box
	if(bbox.GetVolume() == 0)
	{
		const float32 EPSILON = 0.000001f;

		if(bbox.min.x == bbox.max.x)
		{
			bbox.min.x -= EPSILON;
			bbox.max.x += EPSILON;
		}
		if(bbox.min.y == bbox.max.y)
		{
			bbox.min.y -= EPSILON;
			bbox.max.y += EPSILON;
		}
		if(bbox.min.z == bbox.max.z)
		{
			bbox.min.z -= EPSILON;
			bbox.max.z += EPSILON;
		}
	}
}

void Mesh::ComputeNormals()
{
	uint32 i;
	Vector3S e1, e2;
	FixedArray<Vector3S> faceNormals(nIndices / 3);

	//reset normals
	for(i = 0; i < nVertices; i++)
		pVertices[i].normal = Vector3S();

	//compute
	for(i = 0; i < nIndices; i += 3)
	{
		e1 = pVertices[this->pIndices16[i + 1]].position - pVertices[this->pIndices16[i]].position;
		e2 = pVertices[this->pIndices16[i + 2]].position - pVertices[this->pIndices16[i]].position;
		faceNormals[i / 3] = e1.Cross(e2).Normalize();

		pVertices[this->pIndices16[i+0]].normal += faceNormals[i / 3];
		pVertices[this->pIndices16[i+1]].normal += faceNormals[i / 3];
		pVertices[this->pIndices16[i+2]].normal += faceNormals[i / 3];
	}

	//normalize
	for(i = 0; i < nVertices; i++)
		pVertices[i].normal = pVertices[i].normal.Normalize();
}

void Mesh::Subdivide()
{
	uint32 nVerticesNew = this->nVertices + 3 * (this->nIndices / 3); //old vertices + 3 new vertices per face
	this->pVertices = static_cast<Vertex *>(MemRealloc(this->pVertices, nVerticesNew * sizeof(*this->pVertices)));

	uint16 *oldIndices = this->pIndices16;
	uint32 nIndicesNew = 4 * this->nIndices;
	this->pIndices16 = static_cast<uint16 *>(MemAlloc(nIndicesNew * sizeof(*this->pIndices16)));

	Map<uint64, uint16> edgeIndexMap;
	uint16 currentVertexIndex = this->nVertices;
	for(uint16 i = 0; i < this->nIndices; i += 3) //for each face
	{
		uint16 i0 = oldIndices[i + 0];
		uint16 i1 = oldIndices[i + 1];
		uint16 i2 = oldIndices[i + 2];

		uint16 i3 = SubdivEdge(this->pVertices, i0, i1, currentVertexIndex, edgeIndexMap);
		uint16 i4 = SubdivEdge(this->pVertices, i1, i2, currentVertexIndex, edgeIndexMap);
		uint16 i5 = SubdivEdge(this->pVertices, i2, i0, currentVertexIndex, edgeIndexMap);

		//v0, v3, v5
		this->pIndices16[i * 4 + 0] = i0;
		this->pIndices16[i * 4 + 1] = i3;
		this->pIndices16[i * 4 + 2] = i5;

		//v3, v1, v4
		this->pIndices16[i * 4 + 3] = i3;
		this->pIndices16[i * 4 + 4] = i1;
		this->pIndices16[i * 4 + 5] = i4;

		//v5, v4, v2
		this->pIndices16[i * 4 + 6] = i5;
		this->pIndices16[i * 4 + 7] = i4;
		this->pIndices16[i * 4 + 8] = i2;

		//v3, v4, v5
		this->pIndices16[i * 4 + 9] = i3;
		this->pIndices16[i * 4 + 10] = i4;
		this->pIndices16[i * 4 + 11] = i5;
	}

	this->nIndices = nIndicesNew;
	this->nVertices = nVerticesNew;

	if(oldIndices)
		MemFree(oldIndices);
}