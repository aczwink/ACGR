#pragma once
#include <ACStdLib.h>
//Local
#include "../Definitions.h"
#include "../Rendering/IRenderer.h"
#include "../Resources/CMesh.h"

namespace ACGE
{
	//Forward declarations
	class CResourceGroup;

	class ACGE_API CEntity
	{
	private:
		//Members
		CMesh *pMesh;
		CMaterial *pMaterial;

	public:
		//Constructors
		CEntity(const ACStdLib::C8BitString &refResource);
		CEntity(const ACStdLib::C8BitString &refResource, CResourceGroup &refResourceGroup);

		//Methods
		void SetMaterial(const ACStdLib::C8BitString &refResource);
		void SetMaterial(const ACStdLib::C8BitString &refResource, CResourceGroup &refResourceGroup);

		//Inline
		inline const CMaterial *GetMaterial() const
		{
			return this->pMaterial;
		}

		inline const CMesh *GetMesh() const
		{
			return this->pMesh;
		}
	};
}