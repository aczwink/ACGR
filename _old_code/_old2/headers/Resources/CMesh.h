#pragma once
#include <ACStdLib.h>
//Local
#include "../Definitions.h"

namespace ACGE
{
	//Forward declarations
	class CMaterial;
	class CResourceGroup;

	class ACGE_API CMesh
	{
	public:
		struct SVertex
		{
			ACStdLib::Math::CVector3 position;
			ACStdLib::Math::CVector3 normal;
			ACStdLib::Math::CVector2 textureCoords;
		};

	private:
		//Members
		uint32 nVertices;
		SVertex *pVertices;
		uint32 nIndices;
		union
		{
			uint16 *pIndices16;
			uint32 *pIndices32;
		};
		CMaterial *pMaterial;

		//Methods
		void ReadResource(ACStdLib::AInputStream &refInput, CResourceGroup &refResourceGroup);

	protected:
		//Members
		ACStdLib::Math::AxisAlignedBox bbox;

		inline SVertex *GetVertices()
		{
			return this->pVertices;
		}
	
	public:
		//Constructors
		CMesh();
		CMesh(ACStdLib::AInputStream &refInput, CResourceGroup &refResourceGroup);

		//Destructor
		~CMesh();

		//Methods
		void SetIndices(uint16 nIndices, const uint16 *pIndices);
		void SetVertices(uint32 nVertices, const SVertex *pVertices);

		//Inline
		inline const ACStdLib::Math::AxisAlignedBox &GetBoundingBox() const
		{
			return this->bbox;
		}
		
		inline uint16 *GetIndices16() const
		{
			return this->pIndices16;
		}

		inline uint32 *GetIndices32() const
		{
			return this->pIndices32;
		}

		inline const CMaterial *GetMaterial() const
		{
			return this->pMaterial;
		}

		inline uint32 GetNumberOfIndices() const
		{
			return this->nIndices;
		}

		inline uint32 GetNumberOfVertices() const
		{
			return this->nVertices;
		}

		inline const SVertex *GetVertices() const
		{
			return this->pVertices;
		}
	};
}