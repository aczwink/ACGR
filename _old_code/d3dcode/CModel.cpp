//Class Header
#include "CModel.h"
//SJCLib
#include <SJCLib.h>
//Local
#include "CEngine.h"
//Namespaces
using namespace SJCLib;
using namespace CurseGine;

//Constructor
CModel::CModel()
{
	this->pIndexBuffer = NULL;
	this->pVertexBuffer = NULL;
}

//Destructor
CModel::~CModel()
{
	this->Release();
}

//Public Functions
bool CModel::Init()
{
	SVertexType *pVertices;
	uint32 *pIndices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	pVertices = new SVertexType[3];
	pIndices = new uint32[3];

	pVertices[0].position = CVector3(0, 0.5, 0.5);
	pVertices[0].color = CVector4(1, 0, 0, 1);

	pVertices[1].position = CVector3(0.5, -0.5, 0);
	pVertices[1].color = CVector4(0, 1, 0, 1);

	pVertices[2].position = CVector3(-0.5, -0.5, 0.5);
	pVertices[2].color = CVector4(0, 0, 1, 1);

	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;

	MemZero(&vertexBufferDesc, sizeof(vertexBufferDesc));
	MemZero(&indexBufferDesc, sizeof(indexBufferDesc));
	MemZero(&vertexData, sizeof(vertexData));
	MemZero(&indexData, sizeof(indexData));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(SVertexType) * 3;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	
	vertexData.pSysMem = pVertices;

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(uint32) * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	indexData.pSysMem = pIndices;

	if(FAILED(D3D.GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &this->pVertexBuffer)))
		return false;
	if(FAILED(D3D.GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &this->pIndexBuffer)))
		return false;

	HeapDeleteArray(pIndices);
	HeapDeleteArray(pVertices);

	return true;
}

void CModel::Release()
{
	SAFE_RELEASE(this->pIndexBuffer);
	SAFE_RELEASE(this->pVertexBuffer);
}

void CModel::Render()
{
	uint32 stride, offset;

	stride = sizeof(SVertexType);
	offset = 0;

	D3D.GetDeviceContext()->IASetVertexBuffers(0, 1, &this->pVertexBuffer, &stride, &offset);
	D3D.GetDeviceContext()->IASetIndexBuffer(this->pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D.GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}