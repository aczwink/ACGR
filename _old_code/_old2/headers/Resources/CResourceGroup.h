#pragma once
//Local
#include "CResource.h"

namespace ACGE
{
	//Forward declarations
	class CMaterial;
	class CMesh;
	
	class ACGE_API CResourceGroup
	{
	private:
		//Members
		ACStdLib::CMap<ACStdLib::C8BitString, CResource<CMaterial> *> materials;
		ACStdLib::CMap<ACStdLib::C8BitString, CResource<CMesh> *> meshes;
		ACStdLib::CMap<ACStdLib::C8BitString, CResource<CTexture> *> textures;

		//Inline
		inline void AddMaterial(const ACStdLib::C8BitString &refName, ACStdLib::CFileInputStream *pInput)
		{
			this->materials.Insert(refName, new CResource<CMaterial>(*this, pInput));
		}

		inline void AddMesh(const ACStdLib::C8BitString &refName, ACStdLib::CFileInputStream *pInput)
		{
			this->meshes.Insert(refName, new CResource<CMesh>(*this, pInput));
		}
		
		inline void AddMesh(const ACStdLib::C8BitString &refName, CMesh *pMesh)
		{
			this->meshes.Insert(refName, new CResource<CMesh>(*this, pMesh));
		}

		inline void AddTexture(const ACStdLib::C8BitString &refName, ACStdLib::CFileInputStream *pInput)
		{
			this->textures.Insert(refName, new CResource<CTexture>(*this, pInput));
		}

	public:
		//Destructor
		~CResourceGroup();

		//Methods
		void AddLocation(const ACStdLib::CPath &refPath, bool recursive);
		void AddPrimitives();
		CMaterial *GetMaterial(const ACStdLib::C8BitString &refResource);
		CMesh *GetMesh(const ACStdLib::C8BitString &refResource);
		CTexture *GetTexture(const ACStdLib::C8BitString &refResource);
	};
}