//Class Header
#include "../../headers/Scene/CEntity.h"
//Local
#include "../../headers/Resources/CResourceGroup.h"
#include "../../headers/Resources/CResourcesManager.h"
//Namespaces
using namespace ACStdLib;
using namespace ACGE;

//Constructors
CEntity::CEntity(const C8BitString &refResource)
{
	this->pMaterial = nullptr;
	this->pMesh = CResourcesManager::GetInstance().GetMesh(refResource);
}

CEntity::CEntity(const C8BitString &refResource, CResourceGroup &refResourceGroup)
{
	this->pMaterial = nullptr;
	this->pMesh = refResourceGroup.GetMesh(refResource);
}

//Public methods
void CEntity::SetMaterial(const C8BitString &refResource)
{
	this->pMaterial = CResourcesManager::GetInstance().GetMaterial(refResource);
}

void CEntity::SetMaterial(const C8BitString &refResource, CResourceGroup &refResourceGroup)
{
	this->pMaterial = refResourceGroup.GetMaterial(refResource);
}