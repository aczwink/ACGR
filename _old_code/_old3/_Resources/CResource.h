/*
#pragma once
#include <ACStdLib.h>
//Local
#include "../Definitions.h"
#include "ACFF.h"
#include "CMaterial.h"
#include "CMesh.h"
#include "CTexture.h"

namespace ACGE
{
	class CResource
	{
	private:
		//Members
		EACFFTag type;
		union
		{
			CMaterial *pMaterial;
			CMesh *pMesh;
			CTexture *pTexture;
		};

	public:
		//Constructors
		inline CResource(EACFFTag type, ACStdLib::CFileInputStream *pInput)
		{
			this->type = type;
			this->pInput = pInput;

			this->pMesh = nullptr;
		}
		
		//Methods
		CMaterial *LoadMaterial();
		CTexture *LoadTexture();
	};
}
*/