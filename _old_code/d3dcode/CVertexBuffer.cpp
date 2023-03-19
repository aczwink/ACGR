//Class Header
#include "CVertexBuffer.h"
//Local
#include "CD3D.h"
//Namespaces
using namespace ACStdLib;
using namespace ACGE;

//Constructor
CVertexBuffer::CVertexBuffer(uint32 nVertices, uint32 vertexSize, void *pVertices, CD3D &refD3D)
{
	HRESULT hr;
	ID3D11Device *pDevice;
	ID3D11DeviceContext *pImmediateContext;
	D3D11_BUFFER_DESC vertexBufferDesc = {};
	D3D11_SUBRESOURCE_DATA vertexBufferData = {};

	pDevice = refD3D.GetDevice();
	pImmediateContext = refD3D.GetImmediateContext();
	
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.ByteWidth = nVertices * vertexSize;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	vertexBufferData.pSysMem = pVertices;
	
	hr = pDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &this->pBuffer);
	if(FAILED(hr))
	{
		ASSERT(0);
	}
	
	//Set the vertex buffer
	UINT offset = 0;
	pImmediateContext->IASetVertexBuffers(0, 1, &this->pBuffer, &vertexSize, &offset);
}

//Destructor
CVertexBuffer::~CVertexBuffer()
{
	COM_SAFE_RELEASE(this->pBuffer);
}