#pragma once
#include <ACStdLib.h>
//Local
#include "../Definitions.h"
#include "CTexture.h"

namespace ACGE
{
	//Forward declarations
	class CResourceGroup;

	class ACGE_API CMaterial
	{
	private:
		//Members
		CTexture *pTexture;

		//Methods
		void ReadResource(ACStdLib::AInputStream &refInput, CResourceGroup &refResourceGroup);

	public:
		//Members
		bool useAlphaBlending;
		ACStdLib::Math::CVector3 diffuseColor;

		//Constructors
		CMaterial(ACStdLib::AInputStream &refInput, CResourceGroup &refResourceGroup);

		//Inline
		inline const CTexture *GetTexture() const
		{
			return this->pTexture;
		}
	};
}