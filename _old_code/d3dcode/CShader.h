#pragma once
//SJCLib
#include <SJCLib.h>
//Global
#include <D3D11.h>
//Local
#include "CMatrix4x4.h"
#include "Definitions.h"

namespace CurseGine
{
	class CURSEGINE_API CShader
	{
		struct SMatrixBufferType
		{
			CMatrix4x4 world;
			CMatrix4x4 view;
			CMatrix4x4 projection;
		};
	private:
		//Variables
		ID3D11InputLayout *pLayout;
		ID3D11Buffer *pMatrixBuffer;
		ID3D11PixelShader *pPixelShader;
		ID3D11VertexShader *pVertexShader;
		//Functions
		void CompilationError(ID3D10Blob *pErrorMessage, SJCLib::CString shaderFilename);
	public:
		//Constructor
		CShader();
		//Destructor
		~CShader();
		//Functions
		bool Init(SJCLib::CString psFilename, SJCLib::CString vsFilename);
		void Release();
		bool Render(uint32 nIndices, const CMatrix4x4 &worldMatrix, const CMatrix4x4 &viewMatrix, const CMatrix4x4 projectionMatrix);
		bool SetShaderParameters(CMatrix4x4 worldMatrix, CMatrix4x4 viewMatrix, CMatrix4x4 projectionMatrix);
	};
}