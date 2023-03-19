#pragma once
#include <ACStdLib.h>
//Local
#include "../Definitions.h"

namespace ACGE
{
	//Forward declarations
	class CResourceGroup;

	enum ETextureFormat
	{
		TEXTURE_FORMAT_DXT1,
		TEXTURE_FORMAT_DXT5
	};

	class ACGE_API CTexture
	{
	private:
		//Members
		uint16 width;
		uint16 height;
		ETextureFormat textureFormat;
		void *pBuffer;

		//Methods
		void ReadResource(ACStdLib::AInputStream &refInput, CResourceGroup &refResourceGroup);

	public:
		//Constructors
		CTexture(ACStdLib::AInputStream &refInput, CResourceGroup &refResourceGroup);

		//Destructor
		~CTexture();

		//Inline
		inline const void *GetData() const
		{
			return this->pBuffer;
		}

		inline ETextureFormat GetFormat() const
		{
			return this->textureFormat;
		}

		inline uint16 GetHeight() const
		{
			return this->height;
		}

		inline uint16 GetWidth() const
		{
			return this->width;
		}
	};
}