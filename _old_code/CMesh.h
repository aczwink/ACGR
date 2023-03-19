/*
#pragma once

class CMesh
{
public:	
	//Methods
	void ComputeVertexNormals();

	inline uint32 GetNumberOfFaces() const
	{
		return this->faces.GetNumberOfElements();
	}

	inline const CArray<Math::CVector3> &GetPositions() const
	{
		return this->positions;
	}

	inline void SetNumberOfFaces(uint32 nFaces)
	{
		this->faces.Resize(nFaces);
	}

	inline void SetNumberOfNormals(uint32 nNormals)
	{
		this->normals.Resize(nNormals);
	}

	inline void SetNumberOfPositions(uint32 nVertices)
	{
		this->positions.Resize(nVertices);
	}

	inline void SetNumberOfUVs(uint32 nUVs)
	{
		this->uvs.Resize(nUVs);
	}

	inline void SetFace(uint32 index, const SFace &refFace)
	{
		this->faces[index] = refFace;
	}

	inline void SetNormal(uint32 index, const Math::CVector3 &refNormal)
	{
		this->normals[index] = refNormal;
	}

	inline void SetPosition(uint32 index, const Math::CVector3 &refPosition)
	{
		this->positions[index] = refPosition;
	}

	inline void SetTriangle(uint32 index, uint32 materialIndex, uint32 i0, uint32 i1, uint32 i2)
	{
		this->faces[index].faceType = 3;
		this->faces[index].materialIndex = materialIndex;
		this->faces[index].vertexTable[0].positionIndex = i0;
		this->faces[index].vertexTable[0].normalsIndex = i0;
		this->faces[index].vertexTable[0].uvIndex = i0;
		this->faces[index].vertexTable[1].positionIndex = i1;
		this->faces[index].vertexTable[1].normalsIndex = i1;
		this->faces[index].vertexTable[1].uvIndex = i1;
		this->faces[index].vertexTable[2].positionIndex = i2;
		this->faces[index].vertexTable[2].normalsIndex = i2;
		this->faces[index].vertexTable[2].uvIndex = i2;
	}

	inline void SetUV(uint32 index, const Math::CVector2 &refUV)
	{
		this->uvs[index] = refUV;
	}
};
*/