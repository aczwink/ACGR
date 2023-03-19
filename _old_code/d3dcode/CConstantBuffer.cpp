//Class Header
#include "CConstantBuffer.h"
//Local
#include "CD3D.h"
//Namespaces
using namespace ACGE;

//Constructor
CConstantBuffer::CConstantBuffer(uint32 size, CD3D &refD3D) : refD3D(refD3D)
{
	HRESULT hr;
	ID3D11Device *pDevice;
	D3D11_BUFFER_DESC constantBufferDesc = {};

	pDevice = refD3D.GetDevice();
	
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.ByteWidth = size;
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	
	hr = pDevice->CreateBuffer(&constantBufferDesc, NULL, &this->pBuffer);
	if(FAILED(hr))
	{
		ASSERT(0);
	}
}

//Destructor
CConstantBuffer::~CConstantBuffer()
{
	COM_SAFE_RELEASE(this->pBuffer);
}

//Public methods
void CConstantBuffer::Update(const void *pData)
{
	ID3D11DeviceContext *pImmediateContext;

	pImmediateContext = this->refD3D.GetImmediateContext();
	
	pImmediateContext->UpdateSubresource(this->pBuffer, 0, nullptr, pData, 0, 0);
	pImmediateContext->VSSetConstantBuffers(0, 1, &this->pBuffer);
}