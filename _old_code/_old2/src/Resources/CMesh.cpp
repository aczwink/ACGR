//Class Header
#include "../../headers/Resources/CMesh.h"
//Local
#include "../../headers/Resources/CResourceGroup.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Math;
using namespace ACGE;

//Local funtions
static CVector3 ReadVec3(AInputStream &refInput)
{
	CVector3 v;

	v.x = refInput.ReadFloat32LE();
	v.y = refInput.ReadFloat32LE();
	v.z = refInput.ReadFloat32LE();

	return v;
}

//Constructors
CMesh::CMesh()
{
	this->nIndices = 0;
	this->nVertices = 0;
	this->pIndices32 = nullptr;
	this->pVertices = nullptr;
	this->pMaterial = nullptr;
}

CMesh::CMesh(AInputStream &refInput, CResourceGroup &refResourceGroup)
{
	this->ReadResource(refInput, refResourceGroup);
}

//Destructor
CMesh::~CMesh()
{
	if(this->pVertices)
		MemFree(this->pVertices);
	if(this->pIndices32)
		MemFree(this->pIndices32);
}

//Private methods
void CMesh::ReadResource(AInputStream &refInput, CResourceGroup &refResourceGroup)
{
	uint32 i;

	CTextReader reader(refInput);
	
	refInput.Skip(4); //ACFF tag
	refInput.Skip(2); //ACFF type
	
	//read vertices
	this->nVertices = refInput.ReadUInt32LE();
	this->pVertices = (SVertex *)MemAlloc(this->nVertices * sizeof(*this->pVertices));
	
	for(i = 0; i < this->nVertices; i++)
	{
		this->pVertices[i].position.x = refInput.ReadFloat32LE();
		this->pVertices[i].position.y = refInput.ReadFloat32LE();
		this->pVertices[i].position.z = refInput.ReadFloat32LE();
		
		this->pVertices[i].normal.x = refInput.ReadFloat32LE();
		this->pVertices[i].normal.y = refInput.ReadFloat32LE();
		this->pVertices[i].normal.z = refInput.ReadFloat32LE();
		
		this->pVertices[i].textureCoords.x = refInput.ReadFloat32LE();
		this->pVertices[i].textureCoords.y = refInput.ReadFloat32LE();
	}
	
	//read faces
	this->nIndices = refInput.ReadUInt32LE();
	if(this->nVertices > UINT16_MAX)
	{
		this->pIndices32 = (uint32 *)MemAlloc(this->nIndices * sizeof(*this->pIndices32));

		for(i = 0; i < this->nIndices; i++)
		{
			this->pIndices32[i] = refInput.ReadUInt32LE();
		}
	}
	else
	{
		this->pIndices16 = (uint16 *)MemAlloc(this->nIndices * sizeof(*this->pIndices16));

		for(i = 0; i < this->nIndices; i++)
		{
			this->pIndices16[i] = refInput.ReadUInt16LE();
		}
	}

	//read material
	this->pMaterial = refResourceGroup.GetMaterial(reader.ReadASCII_Line());

	//mesh info
	this->bbox.min = ReadVec3(refInput);
	this->bbox.max = ReadVec3(refInput);
}

//Public methods
void CMesh::SetIndices(uint16 nIndices, const uint16 *pIndices)
{
	this->nIndices = nIndices;

	if(this->pIndices32)
		MemFree(this->pIndices32);

	this->pIndices16 = (uint16 *)MemAlloc(nIndices * sizeof(*pIndices));
	MemCopy(this->pIndices16, pIndices, nIndices * sizeof(*pIndices));
}

void CMesh::SetVertices(uint32 nVertices, const SVertex *pVertices)
{
	this->nVertices = nVertices;

	if(this->pVertices)
		MemFree(this->pVertices);

	this->pVertices = (SVertex *)MemAlloc(nVertices * sizeof(*pVertices));
	MemCopy(this->pVertices, pVertices, nVertices * sizeof(*pVertices));
}