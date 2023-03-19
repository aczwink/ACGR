//Class header
#include "../../headers/Resources/CResourceGroup.h"
//Local
#include "Primitives/CCuboid.h"
#include "Primitives/CIcosahedron.h"
#include "Primitives/CPlane.h"
#include "Primitives/CTriangle.h"
//Namespaces
using namespace ACStdLib;
using namespace ACGE;

//Destructor
CResourceGroup::~CResourceGroup()
{
	//release resources
	for(const auto &refKV : this->materials)
	{
		delete refKV.value;
	}
	
	for(const auto &refKV : this->meshes)
	{
		delete refKV.value;
	}
	
	for(const auto &refKV : this->textures)
	{
		delete refKV.value;
	}
}

//Public methods
void CResourceGroup::AddLocation(const CPath &refPath, bool recursive)
{
	for(const CPath &refSubPath : refPath)
	{
		CPath concatPath = refPath / refSubPath;
		C8BitString subNodeName = (char *)CUTF8String(refSubPath.GetTitle().GetUTF16()).GetC_Str();
		
		if(concatPath.IsDirectory())
		{
			if(recursive)
				this->AddLocation(concatPath, recursive);
		}
		else
		{
			CFileInputStream *pInput;
			
			pInput = new CFileInputStream(concatPath);
			
			if(concatPath.GetFileExtension() == "acmat")
				this->AddMaterial(subNodeName, pInput);
			else if(concatPath.GetFileExtension() == "acmsh")
				this->AddMesh(subNodeName, pInput);
			else if(concatPath.GetFileExtension() == "actex")
				this->AddTexture(subNodeName, pInput);
		}
	}
}

void CResourceGroup::AddPrimitives()
{
	this->AddMesh("Box", new CCuboid);
	this->AddMesh("Icosahedron", new CIcosahedron);
	this->AddMesh("Plane", new CPlane);
	this->AddMesh("Triangle", new CTriangle);
}

CMaterial *CResourceGroup::GetMaterial(const C8BitString &refResource)
{
	ASSERT_MSG(this->materials.Contains(refResource), CString("Missing material: \"" + refResource + "\""));

	return this->materials[refResource]->GetStructuredDataObject();
}

CMesh *CResourceGroup::GetMesh(const C8BitString &refResource)
{
	ASSERT_MSG(this->meshes.Contains(refResource), CString("Missing mesh: \"" + refResource + "\""));
	
	return this->meshes[refResource]->GetStructuredDataObject();
}

CTexture *CResourceGroup::GetTexture(const C8BitString &refResource)
{
	ASSERT_MSG(this->textures.Contains(refResource), CString("Missing texture: \"" + refResource + "\""));

	return this->textures[refResource]->GetStructuredDataObject();
}