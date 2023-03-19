/*
using namespace ACStdLib;

//Destructor
CResource::~CResource()
{
	if(this->pInput)
		delete this->pInput;

	switch(this->type)
	{
	case ACFF_TAG_MATERIAL:
		{
			if(this->pMaterial)
				delete this->pMaterial;
		}
		break;
	case ACFF_TAG_MESH_RENDER_OPT:
		{
			if(this->pMesh)
				delete this->pMesh;
		}
		break;
	case ACFF_TAG_TEXTURE:
		{
			if(this->pTexture)
				delete this->pTexture;
		}
		break;
	default:
		ASSERT(0);
	}
}

//Public methods
CMaterial *CResource::LoadMaterial()
{
	ASSERT(this->type == ACFF_TAG_MATERIAL);

	if(!this->pMaterial)
	{
		uint64 offset;

		offset = this->pInput->GetCurrentOffset();
		this->pMaterial = new CMaterial(*this->pInput);
		this->pInput->SetCurrentOffset(offset);
	}

	return this->pMaterial;
}

CTexture *CResource::LoadTexture()
{
	ASSERT(this->type == ACFF_TAG_TEXTURE);

	if(!this->pTexture)
	{
		uint64 offset;

		offset = this->pInput->GetCurrentOffset();
		this->pTexture = new CTexture(*this->pInput);
		this->pInput->SetCurrentOffset(offset);
	}

	return this->pTexture;
}
*/