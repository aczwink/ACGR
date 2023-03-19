#pragma once
//Global
#include <d3d11.h>
//Libs
#include <ACStdLib.h>
//Local
#include "CResourceNode.h"
#include "Definitions.h"

namespace ACGE
{
	//Forward declarations
	class CD3D;

	class ACGE_API CShaderManager
	{
	private:
		//Members
		CD3D &refD3D;
		ACStdLib::CMap<ID3D11DeviceChild *, ID3DBlob *> buffers;
		ACStdLib::CMap<ACStdLib::CString, ID3D11PixelShader *> pixelShaders;
		ACStdLib::CMap<ACStdLib::CString, ID3D11VertexShader *> vertexShaders;
		//Methods
		byte *GetShaderSource(const ACStdLib::CString &refResource, uint32 &refSize);
	public:
		//Constructor
		CShaderManager(CD3D &refD3D);
		//Destructor
		~CShaderManager();
		//Methods
		void CachePixelShader(const ACStdLib::CString &refShader);
		void CacheVertexShader(const ACStdLib::CString &refShader);

		//Inline
		inline ID3D11PixelShader *GetPixelShader(const ACStdLib::CString &refShader)
		{
			this->CachePixelShader(refShader);
			
			return this->pixelShaders[refShader];
		}
		
		inline ID3D11VertexShader *GetVertexShader(const ACStdLib::CString &refShader)
		{
			this->CacheVertexShader(refShader);

			return this->vertexShaders[refShader];
		}

		inline ID3DBlob *GetShaderData(ID3D11DeviceChild *pShader)
		{
			return this->buffers[pShader];
		}
	};
}