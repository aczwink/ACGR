#pragma once
//Local
#include "CResourceGroup.h"

namespace ACGE
{
	class ACGE_API CResourcesManager
	{
	private:
		//Members
		ACStdLib::CArray<CResourceGroup *> resourceGroups;

		//Constructor
		CResourcesManager();

		//Destructor
		~CResourcesManager();

	public:
		//Methods
		CMaterial *GetMaterial(const ACStdLib::C8BitString &refResource);
		CMesh *GetMesh(const ACStdLib::C8BitString &refResource);
		CTexture *GetTexture(const ACStdLib::C8BitString &refResource);

		//Inline
		inline void AppendResourceGroup(CResourceGroup *pResourceGroup)
		{
			this->resourceGroups.Push(pResourceGroup);
		}
		
		//Functions
		static CResourcesManager &GetInstance()
		{
			static CResourcesManager mgr;

			return mgr;
		}
	};
}