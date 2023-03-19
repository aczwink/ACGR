//Class Header
#include "CShader.h"
//Global
#include <D3DX11.h>
//Local
#include "CEngine.h"
#include "CLog.h"
//Namespaces
using namespace SJCLib;
using namespace CurseGine;

//Constructor
CShader::CShader()
{
	this->pLayout = NULL;
	this->pMatrixBuffer = NULL;
	this->pPixelShader = NULL;
	this->pVertexShader = NULL;
}

//Destructor
CShader::~CShader()
{
	this->Release();
}

//Private Functions
void CShader::CompilationError(ID3D10Blob *pErrorMessage, CString shaderFilename)
{
	if(pErrorMessage)
	{
		LOGERROR(CString((char *)pErrorMessage->GetBufferPointer(), (uint32)pErrorMessage->GetBufferSize()));
	}
	else
	{
		LOGERROR("Unknown compilation error, while trying to compile " + shaderFilename);
	}
}

//Public Functions
bool CShader::Init(CString psFilename, CString vsFilename)
{
	HRESULT hr;
	ID3D10Blob *pErrorMessage;
	ID3D10Blob *pPixelShaderBuffer;
	ID3D10Blob *pVertexShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	D3D11_BUFFER_DESC matrixBufferDesc;
	uint32 nElements;

	hr = D3DX11CompileFromFile(vsFilename.GetC_Str(), NULL, NULL, "ColorVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pVertexShaderBuffer, &pErrorMessage, NULL);
	if(FAILED(hr))
	{
		this->CompilationError(pErrorMessage, vsFilename);
		return false;
	}

	hr = D3DX11CompileFromFile(psFilename.GetC_Str(), NULL, NULL, "ColorPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pPixelShaderBuffer, &pErrorMessage, NULL);
	if(FAILED(hr))
	{
		this->CompilationError(pErrorMessage, psFilename);
		return false;
	}

	hr = D3D.GetDevice()->CreateVertexShader(pVertexShaderBuffer->GetBufferPointer(), pVertexShaderBuffer->GetBufferSize(), NULL, &this->pVertexShader);
	if(FAILED(hr))
		return false;

	hr = D3D.GetDevice()->CreatePixelShader(pPixelShaderBuffer->GetBufferPointer(), pPixelShaderBuffer->GetBufferSize(), NULL, &this->pPixelShader);
	if(FAILED(hr))
		return false;

	MemZero(&polygonLayout, sizeof(polygonLayout));

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	nElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	hr = D3D.GetDevice()->CreateInputLayout(polygonLayout, nElements, pVertexShaderBuffer->GetBufferPointer(), pVertexShaderBuffer->GetBufferSize(), &this->pLayout);
	if(FAILED(hr))
		return false;
	SAFE_RELEASE(pPixelShaderBuffer);
	SAFE_RELEASE(pVertexShaderBuffer);
	
	MemZero(&matrixBufferDesc, sizeof(matrixBufferDesc));

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(SMatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	hr = D3D.GetDevice()->CreateBuffer(&matrixBufferDesc, NULL, &pMatrixBuffer);
	if(FAILED(hr))
		return false;

	return true;
}

void CShader::Release()
{
	SAFE_RELEASE(this->pMatrixBuffer);
	SAFE_RELEASE(this->pLayout);
	SAFE_RELEASE(this->pPixelShader);
	SAFE_RELEASE(this->pVertexShader);
}

bool CShader::Render(uint32 nIndices, const CMatrix4x4 &worldMatrix, const CMatrix4x4 &viewMatrix, const CMatrix4x4 projectionMatrix)
{
	if(!this->SetShaderParameters(worldMatrix, viewMatrix, projectionMatrix))
		return false;
	
	D3D.GetDeviceContext()->IASetInputLayout(this->pLayout);
	D3D.GetDeviceContext()->VSSetShader(this->pVertexShader, NULL, 0);
	D3D.GetDeviceContext()->PSSetShader(this->pPixelShader, NULL, 0);
	
	D3D.GetDeviceContext()->DrawIndexed(nIndices, 0, 0);

	return true;
}

bool CShader::SetShaderParameters(CMatrix4x4 worldMatrix, CMatrix4x4 viewMatrix, CMatrix4x4 projectionMatrix)
{
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	SMatrixBufferType *pData;
	uint32 pos;

	worldMatrix.Transpose();
	viewMatrix.Transpose();
	projectionMatrix.Transpose();

	if(FAILED(D3D.GetDeviceContext()->Map(this->pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource)))
		return false;

	pData = (SMatrixBufferType *)mappedSubresource.pData;
	pData->projection = projectionMatrix;
	pData->view = viewMatrix;
	pData->world = worldMatrix;

	D3D.GetDeviceContext()->Unmap(this->pMatrixBuffer, 0);

	pos = 0;

	D3D.GetDeviceContext()->VSSetConstantBuffers(pos, 1, &this->pMatrixBuffer);

	return true;
}