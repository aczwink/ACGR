//Class Header
#include "CIndexBuffer.h"
//Local
#include "CD3D.h"
//Namespaces
using namespace ACStdLib;
using namespace ACGE;

//Constructor
CIndexBuffer::CIndexBuffer(uint32 nIndices, void *pIndices, CD3D &refD3D)
{
	HRESULT hr;
	ID3D11Device *pDevice;
	ID3D11DeviceContext *pImmediateContext;
	D3D11_BUFFER_DESC indexBufferDesc = {};
	D3D11_SUBRESOURCE_DATA indexBufferData = {};

	pDevice = refD3D.GetDevice();
	pImmediateContext = refD3D.GetImmediateContext();

	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.ByteWidth = nIndices * sizeof(uint32);
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	indexBufferData.pSysMem = pIndices;

	hr = pDevice->CreateBuffer(&indexBufferDesc, &indexBufferData, &this->pBuffer);
	if(FAILED(hr))
	{
		ASSERT(0);
	}

	//Set the index buffer
	pImmediateContext->IASetIndexBuffer(this->pBuffer, DXGI_FORMAT_R32_UINT, 0);
}

//Destructor
CIndexBuffer::~CIndexBuffer()
{
	COM_SAFE_RELEASE(this->pBuffer);
}