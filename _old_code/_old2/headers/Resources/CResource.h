#pragma once
#include <ACStdLib.h>
//Local
#include "../Definitions.h"
#include "CMaterial.h"

namespace ACGE
{
	//Forward declarations
	class CResourceGroup;

	template<typename ResourceType>
	class ACGE_API CResource
	{
	private:
		//Members
		CResourceGroup &refGroup;
		ACStdLib::CFileInputStream *pInput;
		ResourceType *pStructuredDataObject;
	
	public:
		//Constructors
		inline CResource(CResourceGroup &refGroup, ACStdLib::CFileInputStream *pInput) : refGroup(refGroup)
		{
			this->pInput = pInput;
			this->pStructuredDataObject = nullptr;
		}
		
		inline CResource(CResourceGroup &refGroup, ResourceType *pStructuredDataObject) : refGroup(refGroup)
		{
			this->pStructuredDataObject = pStructuredDataObject;
			this->pInput = nullptr;
		}

		//Destructor
		~CResource();

		//Inline
		inline ResourceType *GetStructuredDataObject()
		{
			if(!this->pStructuredDataObject)
				this->pStructuredDataObject = new ResourceType(*this->pInput, this->refGroup);
			
			return this->pStructuredDataObject;
		}
	};
}