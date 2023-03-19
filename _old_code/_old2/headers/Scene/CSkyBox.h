#pragma once

namespace ACGE
{
	//Forward declarations
	class CTexture;
	
	class CSkyBox
	{
	public:
		//Members
		CTexture *pLeft;
		CTexture *pRight;
		CTexture *pBottom;
		CTexture *pTop;
		CTexture *pBack;
		CTexture *pFront;

		//Constructor
		inline CSkyBox()
		{
			this->pLeft = nullptr;
			this->pRight = nullptr;
			this->pBottom = nullptr;
			this->pTop = nullptr;
			this->pBack = nullptr;
			this->pFront = nullptr;
		}
	};
}