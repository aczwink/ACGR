/*
//Class Header
#include "../../headers/_Resources/CResourceNode.h"
//Namespaces
using namespace ACGE;

//Constructors
CResourceNode::CResourceNode()
{
}

//Destructor
CResourceNode::~CResourceNode()
{
//release children
	for(const auto &refKV : this->children)
	{
		delete refKV.value;
	}
}

//Private methods
bool CResourceNode::GetNextChild(const CString &refResource, CString &refChildName, CString &refRest)
{
	ASSERT(0);
	/*
	int32 pos;

	pos = refResource.Find('/');
	if(pos != UINT32_MAX)
	{
		refChildName = refResource.SubString(0, pos);
		refRest = refResource.SubString(pos + 1, refResource.GetLength() - pos - 1);

		ASSERTMSG(this->children.Contains(refChildName), refResource.GetC_Str());
		
		return true;
	}
	
	return false;
}

//Public methods
CMaterial *CResourceNode::LoadMaterial(const CString &refResource)
{
	ASSERT(0);
	/*
	CString childName, rest;

	if(this->GetNextChild(refResource, childName, rest))
	{
		return this->children[childName]->LoadMaterial(rest);
	}

	ASSERT(this->materials.Contains(refResource));

	return this->materials[refResource]->LoadMaterial();
	return nullptr;
}

CMesh *CResourceNode::LoadMesh(const CString &refResource)
{
	/*
	CString childName, rest;
	
	if(this->GetNextChild(refResource, childName, rest))
	{
		return this->children[childName]->LoadMesh(rest);
	}

	ASSERT(0);
	return nullptr;
}

CTexture *CResourceNode::LoadTexture(const CString &refResource)
{
	/*
	CString childName, rest;

	if(this->GetNextChild(refResource, childName, rest))
	{
		return this->children[childName]->LoadTexture(rest);
	}

	ASSERT(this->textures.Contains(refResource));

	return this->textures[refResource]->LoadTexture();
	ASSERT(0);
	return nullptr;
}
*/