/*
//Class header
#include "CMesh.h"
//Namespaces
using namespace ACStdLib::Math;

//Public methods
void CMesh::ComputeVertexNormals()
{
	uint32 i;
	CVector3 edge1, edge2, faceNormal;
	
	//one normal per position
	this->SetNumberOfNormals(this->GetNumberOfPositions());
	//reset normals
	for(i = 0; i < this->GetNumberOfNormals(); i++)
		this->normals[i] = CVector3();

	//calculate face normals
	for(SFace &refFace : this->faces)
	{
		edge1 = this->positions[refFace.vertexTable[1].positionIndex] - this->positions[refFace.vertexTable[0].positionIndex];
		edge2 = this->positions[refFace.vertexTable[2].positionIndex] - this->positions[refFace.vertexTable[0].positionIndex];
		
		faceNormal = edge1.Cross(edge2).Normalize();

		for(i = 0; i < refFace.faceType; i++)
		{
			refFace.vertexTable[i].normalsIndex = refFace.vertexTable[i].positionIndex;
			
			this->normals[refFace.vertexTable[i].normalsIndex] += faceNormal;
		}
	}

	//normalize
	for(i = 0; i < this->GetNumberOfNormals(); i++)
		this->normals[i] = this->normals[i].Normalize();
}
*/