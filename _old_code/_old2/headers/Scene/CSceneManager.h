#pragma once
//Local
#include "CCamera.h"
#include "CSceneNode.h"
#include "CSkyBox.h"

namespace ACGE
{
	class ACGE_API CSceneManager
	{
	private:
		//Members
		CSceneNode *pSceneGraphRoot;
		CSkyBox skyBox;
	
	public:
		//Members
		ACStdLib::Math::CVector3 ambientLight;

		//Constructor
		CSceneManager();

		//Destructor
		~CSceneManager();

		//Methods
		void Load(ACStdLib::AInputStream &refInput);
		void SetSkyBox(const ACStdLib::C8BitString &refResource);
		
		//Inline
		inline ACStdLib::CMap<CLight *, CSceneNode *> GetLights()
		{
			return this->pSceneGraphRoot->GetLights();
		}

		inline ACStdLib::CMap<CLight *, CSceneNode *> GetLights() const
		{
			return this->pSceneGraphRoot->GetLights();
		}
		
		inline CSceneNode *GetRootNode()
		{
			return this->pSceneGraphRoot;
		}

		inline const CSceneNode *GetRootNode() const
		{
			return this->pSceneGraphRoot;
		}

		inline const CSkyBox &GetSkyBox() const
		{
			return this->skyBox;
		}
	};
}