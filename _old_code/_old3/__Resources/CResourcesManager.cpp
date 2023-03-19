/*
//Destructor
CResourcesManager::~CResourcesManager()
{
}

//Public methods
CMaterial *CResourcesManager::LoadMaterial(const CString &refResource)
{
	CResource *pResource;

	pResource = this->rootNode.FindResource(refResource);
	ASSERT(pResource);
	
	return pResource->GetMaterial("data/" + refResource);
}

CTexture *CResourcesManager::LoadTexture(const CString &refResource)
{
	CResource *pResource;

	pResource = this->rootNode.FindResource(refResource);
	ASSERT(pResource);

	return pResource->GetTexture("data/" + refResource);
}

CBufferInputStream CResourcesManager::LoadResource(const CString &refResource)
{
	CResource *pResource;

	pResource = this->rootNode.FindResource(refResource);
	ASSERT(pResource);
	
	return pResource->GetStream("data/" + refResource);
}

void CResourcesManager::ReleaseResource(const CString &refResource)
{
	CResource *pResource;

	pResource = this->rootNode.FindResource(refResource);
	ASSERT(pResource);

	pResource->Release();
	pResource->ReleaseAggregate();
}
*/