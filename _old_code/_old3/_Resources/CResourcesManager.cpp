/*
//Class Header
#include "../../headers/_Resources/CResourcesManager.h"
//Namespaces
using namespace ACStdLib;
using namespace ACGE;

//Public methods
void CResourcesManager::LoadResources(const CPath &refPath)
{
	CResourceNode *pNode;
	CUTF8String pathUTF8;
	C8BitString nodeName;
	
	pathUTF8 = CUTF8String(refPath.GetTitle().GetUTF16());
	nodeName = (char *)pathUTF8.GetC_Str();
	
	pNode = new CResourceNode(refPath);
	
	this->AddChild(nodeName, pNode);
}
*/