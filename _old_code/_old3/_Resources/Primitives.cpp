/*
//Local
#include "../../headers/_Resources/CResourceNode.h"
#include "Primitives/CPlane.h"
//Namespaces
using namespace ACGE;

void AddPrimitives(CResourceNode *pParent)
{
	CResourceNode *pNode;
	
	pNode = new CResourceNode();
	pParent->AddChild("Primitives", pNode);
	
	pNode->AddResource("Plane", new CPlane);
}
*/