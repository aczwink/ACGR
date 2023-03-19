//Class Header
#include "CInputLayout.h"
//Local
#include "CD3D.h"
//Namespace
using namespace ACGE;

//Constructor
CInputLayout::CInputLayout(uint32 nLayoutEntries, D3D11_INPUT_ELEMENT_DESC *pLayoutDesc, ID3DBlob *pVertexShaderBuffer, CD3D &refD3D)
{
	HRESULT hr;
	ID3D11Device *pDevice;
	ID3D11DeviceContext *pImmediateContext;

	this->pInputLayout = NULL;

	pDevice = refD3D.GetDevice();
	pImmediateContext = refD3D.GetImmediateContext();
	
	//Create the Input Layout
	hr = pDevice->CreateInputLayout(pLayoutDesc, nLayoutEntries, pVertexShaderBuffer->GetBufferPointer(), pVertexShaderBuffer->GetBufferSize(), &this->pInputLayout);
	if(FAILED(hr))
	{
		ASSERT(0);
	}
	
	//Set the Input Layout
	pImmediateContext->IASetInputLayout(this->pInputLayout);

	//Set Primitive Topology
	pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

//Destructor
CInputLayout::~CInputLayout()
{
	COM_SAFE_RELEASE(this->pInputLayout);
}