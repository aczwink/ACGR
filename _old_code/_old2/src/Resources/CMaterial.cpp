//Class header
#include "../../headers/Resources/CMaterial.h"
//Local
#include "../../headers/Resources/CResourceGroup.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Math;
using namespace ACGE;

//Local funtions
static CVector3 ReadVec3(AInputStream &refInput)
{
	CVector3 v;

	v.x = refInput.ReadFloat32LE();
	v.y = refInput.ReadFloat32LE();
	v.z = refInput.ReadFloat32LE();

	return v;
}

//Constructors
CMaterial::CMaterial(AInputStream &refInput, CResourceGroup &refResourceGroup)
{
	this->ReadResource(refInput, refResourceGroup);
}

//Private methods
void CMaterial::ReadResource(AInputStream &refInput, CResourceGroup &refResourceGroup)
{
	CTextReader reader(refInput);

	//file signature
	refInput.Skip(6);

	//read header
	this->useAlphaBlending = refInput.ReadByte() != 0;

	//read body
	this->diffuseColor = ReadVec3(refInput);
	this->pTexture = refResourceGroup.GetTexture(reader.ReadASCII_ZeroTerminated()); //TODO: what happens if not ascii??
}