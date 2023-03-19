//Class Header
#include "CShaderManager.h"
//Global
#include <d3dcompiler.h>
//Local
#include "CDataManager.h"
#include "CD3D.h"
//Namespaces
using namespace ACStdLib;
using namespace ACGE;

//Constructor
CShaderManager::CShaderManager(CD3D &refD3D) : refD3D(refD3D)
{
}

//Destructor
CShaderManager::~CShaderManager()
{
	for(auto &refKV : this->pixelShaders)
	{
		refKV.value->Release();
	}

	for(auto &refKV : this->vertexShaders)
	{
		refKV.value->Release();
	}

	for(auto &refKV : this->buffers)
	{
		refKV.value->Release();
	}
}

//Private methods
byte *CShaderManager::GetShaderSource(const CString &refResource, uint32 &refSize)
{
	byte *pData;
	
	CBufferInputStream shaderResource(GetResource(refResource));
	
	refSize = shaderResource.GetSize();
	pData = (byte *)MemAlloc(refSize + 1);
	shaderResource.ReadBytes(pData, refSize);
	pData[refSize] = 0;
	
	CDataManager::Get().Release(refResource);
	
	return pData;
}

//Public methods
void CShaderManager::CachePixelShader(const CString &refShader)
{
	byte *pData;
	uint32 shaderSize;
	ID3DBlob *pShaderData;
	ID3D11PixelShader *pPixelShader;
	HRESULT hr;

	if(this->pixelShaders.Contains(refShader))
		return;

	pData = this->GetShaderSource("shaders/pixel/" + refShader, shaderSize);
	hr = D3DCompile(pData, shaderSize, ("shaders/pixel/" + refShader).GetC_Str(), NULL, NULL, "PS", "ps_5_0", 0, 0, &pShaderData, NULL);
	MemFree(pData);
	if(FAILED(hr))
	{
		ASSERT(0);
	}
	
	hr = this->refD3D.GetDevice()->CreatePixelShader(pShaderData->GetBufferPointer(), pShaderData->GetBufferSize(), NULL, &pPixelShader);
	if(FAILED(hr))
	{
		ASSERT(0);
	}
	this->pixelShaders.Insert(refShader, pPixelShader);
	this->buffers.Insert(pPixelShader, pShaderData);
}

void CShaderManager::CacheVertexShader(const CString &refShader)
{
	byte *pData;
	uint32 shaderSize;
	ID3DBlob *pShaderData;
	ID3D11VertexShader *pVertexShader;
	HRESULT hr;

	if(this->vertexShaders.Contains(refShader))
		return;

	pData = this->GetShaderSource("shaders/vertex/" + refShader, shaderSize);
	hr = D3DCompile(pData, shaderSize, ("shaders/vertex/" + refShader).GetC_Str(), NULL, NULL, "VS", "vs_5_0", 0, 0, &pShaderData, NULL);
	MemFree(pData);
	if(FAILED(hr))
	{
		ASSERT(0);
	}
	
	hr = this->refD3D.GetDevice()->CreateVertexShader(pShaderData->GetBufferPointer(), pShaderData->GetBufferSize(), NULL, &pVertexShader);
	if(FAILED(hr))
	{
		ASSERT(0);
	}
	this->vertexShaders.Insert(refShader, pVertexShader);
	this->buffers.Insert(pVertexShader, pShaderData);
}