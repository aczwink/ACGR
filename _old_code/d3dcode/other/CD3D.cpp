//Class Header
#include "CD3D.h"
//Libs
#include <ACStdLib.h>
//Local
#include "CConfig.h"
#include "Definitions.h"
//Namespaces
using namespace ACStdLib;
using namespace ACGE;

//Constructor
CD3D::CD3D()
{
	this->pDevice = NULL;
	this->pImmediateContext = NULL;
	this->pRenderTargetView = nullptr;
	this->pSwapChain = NULL;
}

//Destructor
CD3D::~CD3D()
{
	COM_SAFE_RELEASE(this->pDepthStencilBuffer);
	COM_SAFE_RELEASE(this->pDepthStencilView);
	COM_SAFE_RELEASE(this->pRenderTargetView);
	COM_SAFE_RELEASE(this->pImmediateContext);
	COM_SAFE_RELEASE(this->pSwapChain);
	COM_SAFE_RELEASE(this->pDevice);
}

//Public methods
bool CD3D::Init(HWND hWnd)
{
	uint32 createDeviceFlags;
	HRESULT hr;
	ID3D11Texture2D *pBackBuffer;
	DXGI_MODE_DESC modeDesc;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	createDeviceFlags = 0;
	pBackBuffer = nullptr;

#ifdef _DEBUG
	createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif

	CConfig &refConfig = CConfig::Get();
	
	//Fill out mode desc
	MemZero(&modeDesc, sizeof(modeDesc));

	modeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	modeDesc.Height = refConfig.GetResolutionHeight();
	modeDesc.RefreshRate.Numerator = 60; //60 Hz
	modeDesc.RefreshRate.Denominator = 1;
	modeDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
	modeDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	modeDesc.Width = refConfig.GetResolutionWidth();

	//Fill out swap chain desc
	MemZero(&swapChainDesc, sizeof(swapChainDesc));
	
	swapChainDesc.BufferCount = 1; //one backbuffer
	swapChainDesc.BufferDesc = modeDesc;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.Flags = 0;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed = refConfig.IsWindowed();
	
	hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, nullptr, 0, D3D11_SDK_VERSION, &swapChainDesc, &this->pSwapChain, &this->pDevice, nullptr, &this->pImmediateContext);
	if(FAILED(hr))
	{
		MessageBox(NULL, "D3D11CreateDeviceAndSwapChain failed", "Error", MB_ICONERROR);
		return false;
	}

	//Get back buffer
	hr = this->pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&pBackBuffer);
	if(FAILED(hr))
	{
		MessageBox(NULL, "this->pSwapChain->GetBuffer failed", "Error", MB_ICONERROR);
		return false;
	}

	//Create render target (connect with back buffer)
	hr = this->pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &this->pRenderTargetView);
	COM_SAFE_RELEASE(pBackBuffer);
	if(FAILED(hr))
	{
		MessageBox(NULL, "this->pDevice->CreateRenderTargetView failed", "Error", MB_ICONERROR);
		return false;
	}
	
	//Describe our Depth/Stencil Buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = refConfig.GetResolutionWidth();
	depthStencilDesc.Height = refConfig.GetResolutionHeight();
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	
	this->pDevice->CreateTexture2D(&depthStencilDesc, NULL, &this->pDepthStencilBuffer);
	this->pDevice->CreateDepthStencilView(this->pDepthStencilBuffer, NULL, &this->pDepthStencilView);

	//set render target view with the depth/stencil view as render target
	this->pImmediateContext->OMSetRenderTargets(1, &this->pRenderTargetView, this->pDepthStencilView);

	//Create the Viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.Width = refConfig.GetResolutionWidth();
	viewport.Height = refConfig.GetResolutionHeight();

	//Set the Viewport
	pImmediateContext->RSSetViewports(1, &viewport);

	return true;
}