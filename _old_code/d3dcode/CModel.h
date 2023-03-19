#pragma once
//Global
#include <d3d11.h>
//Local
#include "CVector3.h"
#include "CVector4.h"
#include "Definitions.h"

namespace CurseGine
{
	class CURSEGINE_API CModel
	{
		struct SVertexType
		{
			CVector3 position;
			CVector4 color;
		};
	private:
		//Variables
		ID3D11Buffer *pIndexBuffer;
		ID3D11Buffer *pVertexBuffer;
	public:
		//Constructor
		CModel();
		//Destructor
		~CModel();
		//Functions
		bool Init();
		void Release();
		void Render();
	};
}