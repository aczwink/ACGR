//Class header
#include "../../headers/Resources/CTexture.h"
//Namespaces
using namespace ACStdLib;
using namespace ACGE;

//Constructors
CTexture::CTexture(AInputStream &refInput, CResourceGroup &refResourceGroup)
{
	this->ReadResource(refInput, refResourceGroup);
}

//Destructor
CTexture::~CTexture()
{
	MemFree(pBuffer);
}

//Private methods
void CTexture::ReadResource(AInputStream &refInput, CResourceGroup &refResourceGroup)
{
	uint32 size;

	//read file header
	refInput.Skip(6);

	//read texture header
	width = refInput.ReadUInt16LE();
	height = refInput.ReadUInt16LE();
	this->textureFormat = (ETextureFormat)refInput.ReadByte();
	size = refInput.ReadUInt32LE();
	
	//read image
	pBuffer = (byte *)MemAlloc(size);
	refInput.ReadBytes(pBuffer, size);
}