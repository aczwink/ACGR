#pragma once
#include <ACStdLib.h>

#include "CTexture.h"

namespace ACGE
{
	class ACGE_API CMaterial
	{
	public:
		//Constructors
		inline CMaterial()
		{
			this->pTexture = nullptr;
		}
		
		inline void SetTexture(CTexture *pTexture)
		{
			this->pTexture = pTexture;
		}
	};
}