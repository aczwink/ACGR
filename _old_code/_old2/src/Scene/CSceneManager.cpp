//Class header
#include "../../headers/Scene/CSceneManager.h"
//Local
#include "../../headers/Resources/CResourcesManager.h"
#include "../Internal/Parse.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Math;
using namespace ACGE;

//Constructor
CSceneManager::CSceneManager()
{
	this->pSceneGraphRoot = new CSceneNode;
}

//Destructor
CSceneManager::~CSceneManager()
{
	delete this->pSceneGraphRoot;
}

//Public methods
void CSceneManager::Load(AInputStream &refInput)
{
	XML::CDocument *pDocument;
	
	pDocument = XML::CDocument::Parse(refInput);

	XML::CElement &refScene = pDocument->GetRootElement();

	if(refScene.HasAttribute("ambientLight"))
		this->ambientLight = ParseVec3(refScene.GetAttribute("ambientLight"));

	if(refScene.HasAttribute("skybox"))
		this->SetSkyBox(To8BitString(refScene.GetAttribute("skybox")));
	
	this->pSceneGraphRoot->Deserialize(refScene);

	delete pDocument;
}

void CSceneManager::SetSkyBox(const C8BitString &refResource)
{
	CResourcesManager &refResourceManager = CResourcesManager::GetInstance();
	
	this->skyBox.pBack = refResourceManager.GetTexture(refResource + "_back");
	this->skyBox.pBottom = refResourceManager.GetTexture(refResource + "_bottom");
	this->skyBox.pFront = refResourceManager.GetTexture(refResource + "_front");
	this->skyBox.pLeft = refResourceManager.GetTexture(refResource + "_left");
	this->skyBox.pRight = refResourceManager.GetTexture(refResource + "_right");
	this->skyBox.pTop = refResourceManager.GetTexture(refResource + "_top");
}