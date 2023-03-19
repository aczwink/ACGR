/*
#pragma once
//Local
#include "CResource.h"

namespace ACGE
{
	class ACGE_API CResourceNode
	{
	private:
		//Members
		ACStdLib::CMap<ACStdLib::C8BitString, CResourceNode *> children;
		
		//Methods
		bool GetNextChild(const ACStdLib::CString &refResource, ACStdLib::CString &refChildName, ACStdLib::CString &refRest);
	
	public:
		//Constructors
		CResourceNode();
		CResourceNode(const ACStdLib::CPath &refPath);
		
		//Methods
		CMaterial *LoadMaterial(const ACStdLib::CString &refResource);
		CTexture *LoadTexture(const ACStdLib::CString &refResource);
		
		//Inline
		inline void AddChild(const ACStdLib::C8BitString &refName, CResourceNode *pNode)
		{
			this->children[refName] = pNode;
		}
	};
}
*/