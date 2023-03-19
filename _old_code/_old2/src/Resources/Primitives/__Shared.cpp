//Corresponding header
#include "__Shared.h"

//Constants
#define EPSILON 0.000001f

//Global functions
AxisAlignedBox ComputeBBox(uint32 nVertices, const CMesh::SVertex *pVertices)
{
	AxisAlignedBox bbox;

	for(uint32 i = 0; i < nVertices; i++)
		bbox.Merge(pVertices[i].position);

	//check for empty box
	/*
	if(bbox.GetVolume() == 0)
	{
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
	*/

	return bbox;
}

void ComputeNormals(uint32 nVertices, CMesh::SVertex *pVertices, uint32 nIndices, uint16 *pIndices)
{
	uint32 i;
	CVector3 e1, e2;
	CArray<CVector3> faceNormals;

	faceNormals.Resize(nIndices / 3);

	//reset normals
	for(i = 0; i < nVertices; i++)
		pVertices[i].normal = CVector3();

	//compute
	for(i = 0; i < nIndices; i += 3)
	{
		e1 = pVertices[pIndices[i + 1]].position - pVertices[pIndices[i]].position;
		e2 = pVertices[pIndices[i + 2]].position - pVertices[pIndices[i]].position;
		faceNormals[i / 3] = Cross(e1, e2).Normalize();

		pVertices[pIndices[i]].normal += faceNormals[i / 3];
		pVertices[pIndices[i+1]].normal += faceNormals[i / 3];
		pVertices[pIndices[i+2]].normal += faceNormals[i / 3];
	}

	//normalize
	for(i = 0; i < nVertices; i++)
		pVertices[i].normal = pVertices[i].normal.Normalize();
}