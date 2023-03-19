//Class Header
#include "CD3D.h"
//Namespaces
using namespace SJCLib;
using namespace CurseGine;

//Constructor
CD3D::CD3D()
{
	this->isVSyncEnabled = false;
	this->pDepthStencilBuffer = NULL;
	this->pDepthStencilState = NULL;
	this->pDepthStencilView = NULL;
	this->pDevice = NULL;
	this->pDeviceContext = NULL;
	this->pRenderTargetView = NULL;
	this->pSwapChain = NULL;
}

//Destructor
CD3D::~CD3D()
{
	this->Release();
}

//Public Functions
void CD3D::BeginScene(CColor clearColor)
{
	this->pDeviceContext->ClearRenderTargetView(this->pRenderTargetView, clearColor.GetFloat4());
	//this->pDeviceContext->ClearDepthStencilView(this->pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void CD3D::EndScene()
{
	if(this->isVSyncEnabled)
		this->pSwapChain->Present(1, 0);
	else
		this->pSwapChain->Present(0, 0);
}

ID3D11Device *CD3D::GetDevice() const
{
	return this->pDevice;
}

ID3D11DeviceContext *CD3D::GetDeviceContext() const
{
	return this->pDeviceContext;
}

const CMatrix4x4 &CD3D::GetProjectionMatrix() const
{
	return this->projectionMatrix;
}

const CMatrix4x4 &CD3D::GetWorldMatrix() const
{
	return this->worldMatrix;
}

bool CD3D::Init(HWND hWnd, uint32 screenWidth, uint32 screenHeight, bool useVSync, bool fullscreen, float screenDepth, float screenNear)
{
	D3D_FEATURE_LEVEL featureLevel;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ID3D11Texture2D *pBackBuffer;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	uint32 createDeviceFlags;

	createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif

	this->isVSyncEnabled = useVSync;
	
	//Create the swap chain
	MemZero(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = screenWidth;
	swapChainDesc.BufferDesc.Height = screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	
	if(useVSync)
	{
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	}
	
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = !fullscreen;
	
	featureLevel = D3D_FEATURE_LEVEL_11_0;
	
	if(FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, &this->pSwapChain, &this->pDevice, NULL, &this->pDeviceContext)))
		return false;

	//Create render target view
	if(FAILED(this->pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&pBackBuffer)))
		return false;

	if(FAILED(this->pDevice->CreateRenderTargetView(pBackBuffer, NULL, &this->pRenderTargetView)))
		return false;

	SAFE_RELEASE(pBackBuffer);

	this->pDeviceContext->OMSetRenderTargets(1, &this->pRenderTargetView, NULL);

	/*//Set depth stencil state
	memzero(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	if(FAILED(this->pDevice->CreateTexture2D(&depthBufferDesc, NULL, &this->pDepthStencilBuffer)))
		return false;

	memzero(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	if(FAILED(this->pDevice->CreateDepthStencilState(&depthStencilDesc, &this->pDepthStencilState)))
		return false;

	this->pDeviceContext->OMSetDepthStencilState(this->pDepthStencilState, 1);

	//Create depth stencil view
	memzero(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	if(FAILED(this->pDevice->CreateDepthStencilView(this->pDepthStencilBuffer, &depthStencilViewDesc, &this->pDepthStencilView)))
		return false;

	this->pDeviceContext->OMSetRenderTargets(1, &this->pRenderTargetView, this->pDepthStencilView);

	//Set rasterizer state
	memzero(&rasterDesc, sizeof(rasterDesc));
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;

	if(FAILED(this->pDevice->CreateRasterizerState(&rasterDesc, &this->pRasterizerState)))
		return false;

	this->pDeviceContext->RSSetState(this->pRasterizerState);*/

	//Setup viewport
	MemZero(&viewport, sizeof(viewport));
	viewport.Width = (float)screenWidth;
	viewport.Height = (float)screenHeight;
	viewport.MaxDepth = 1.0f;

	this->pDeviceContext->RSSetViewports(1, &viewport);
	
	//Init Matrices
	this->projectionMatrix.PerspectiveFovLH(PI / 4, (float)screenWidth / (float)screenHeight, screenNear, screenDepth);
	this->worldMatrix.Identity();

	return true;
}

void CD3D::Release()
{
	if(this->pDeviceContext)
		this->pDeviceContext->ClearState();

	SAFE_RELEASE(this->pRenderTargetView);
	SAFE_RELEASE(this->pSwapChain);
	SAFE_RELEASE(this->pDeviceContext);
	SAFE_RELEASE(this->pDevice);
	
	/*
	SAFE_RELEASE(this->pRasterizerState);
	SAFE_RELEASE(this->pDepthStencilView);
	SAFE_RELEASE(this->pDepthStencilState);
	SAFE_RELEASE(this->pDepthStencilBuffer);
	*/
}