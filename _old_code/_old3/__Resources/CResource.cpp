/*
//Local
#include "../../headers/Resources/CMaterial.h"
#include "../../headers/Resources/CMesh.h"

//Constructor
CResource::CResource(EACFFTag type)
{
	this->type = type;
	this->pBuffer = nullptr;
	this->pAggregated = nullptr;
}

//Public methods
CMaterial *CResource::GetMaterial(const CString &refPath)
{
	ASSERT(this->type == ACFF_TAG_MATERIAL);
	if(!this->pAggregated)
	{
		this->pAggregated = new CMaterial(this->GetStream(refPath));
		
		this->Release(); //dont need the raw data anymore
	}

	return (CMaterial *)this->pAggregated;
}

CTexture *CResource::GetTexture(const CString &refPath)
{
	ASSERT(this->type == ACFF_TAG_TEXTURE);
	if(!this->pAggregated)
	{
		this->pAggregated = new CTexture(this->GetStream(refPath));

		this->Release(); //dont need the raw data anymore
	}

	return (CTexture *)this->pAggregated;
}

CBufferInputStream CResource::GetStream(const CString &refPath)
{
	if(!this->pBuffer)
	{
		CFileInputStream file = CPath(refPath);
		
		this->size = (uint32)file.GetSize();
		this->pBuffer = (byte *)MemAlloc(this->size);
		file.ReadBytes(this->pBuffer, this->size);
	}

	return CBufferInputStream(this->pBuffer, this->size);
}

void CResource::Release()
{
	if(this->pBuffer)
	{
		MemFree(this->pBuffer);
		this->pBuffer = nullptr;
	}
}

void CResource::ReleaseAggregate()
{
	if(this->pAggregated)
	{
		switch(this->type)
		{
		case ACFF_TAG_MATERIAL:
			{
				delete (CMaterial *)this->pAggregated;
			}
			break;
		case ACFF_TAG_MESH_RENDER_OPT:
			{
				delete (CMesh *)this->pAggregated;
			}
			break;
		case ACFF_TAG_TEXTURE:
			{
				delete (CTexture *)this->pAggregated;
			}
			break;
		default:
			ASSERT(0);
		}

		this->pAggregated = nullptr;
	}
}
*/