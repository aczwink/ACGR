//Class Header
#include "../../headers/Resources/CResource.h"
//Local
#include "../../headers/Resources/CMesh.h"
//Namespaces
using namespace ACGE;

//Destructor
template<typename ResourceType>
CResource<ResourceType>::~CResource()
{
	if(this->pInput)
		delete this->pInput;

	if(this->pStructuredDataObject)
		delete this->pStructuredDataObject;
}

//Instantiation
template class CResource<CMaterial>;
template class CResource<CMesh>;
template class CResource<CTexture>;