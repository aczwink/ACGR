//Class header
#include "../../headers/Resources/CResourcesManager.h"
//Namespaces
using namespace ACStdLib;
using namespace ACGE;

//Constructor
CResourcesManager::CResourcesManager()
{
}

//Destructor
CResourcesManager::~CResourcesManager()
{
	for(CResourceGroup *const& refpGroup : this->resourceGroups)
	{
		delete refpGroup;
	}
}

//Public methods
CMaterial *CResourcesManager::GetMaterial(const C8BitString &refResource)
{
	for(CResourceGroup *const& refpGroup : this->resourceGroups)
	{
		CMaterial *const& refpMaterial = refpGroup->GetMaterial(refResource);
		if(refpMaterial)
			return refpMaterial;
	}
	
	return nullptr;
}

CMesh *CResourcesManager::GetMesh(const C8BitString &refResource)
{
	for(CResourceGroup *const& refpGroup : this->resourceGroups)
	{
		CMesh *const& refpMesh = refpGroup->GetMesh(refResource);
		if(refpMesh)
			return refpMesh;
	}

	return nullptr;
}

CTexture *CResourcesManager::GetTexture(const C8BitString &refResource)
{
	for(CResourceGroup *const& refpGroup : this->resourceGroups)
	{
		CTexture *const& refpTexture = refpGroup->GetTexture(refResource);
		if(refpTexture)
			return refpTexture;
	}

	return nullptr;
}