#pragma once
//Global
#include <d3d11.h>
//SJCLib
#include <SJCLib.h>
//Local
#include "CColor.h"
#include "CMatrix4x4.h"
#include "Definitions.h"

namespace CurseGine
{
	class CURSEGINE_API CD3D
	{
	private:
		//Variables
		ID3D11Texture2D *pDepthStencilBuffer;
		ID3D11DepthStencilState *pDepthStencilState;
		ID3D11DepthStencilView *pDepthStencilView;
		ID3D11Device *pDevice;
		ID3D11DeviceContext *pDeviceContext;
		ID3D11RasterizerState *pRasterizerState;
		ID3D11RenderTargetView *pRenderTargetView;
		IDXGISwapChain *pSwapChain;
		bool isVSyncEnabled;
		CMatrix4x4 worldMatrix;
		CMatrix4x4 projectionMatrix;
	public:
		//Constructor
		CD3D();
		//Destructor
		~CD3D();
		//Functions
		void BeginScene(CColor clearColor);
		void EndScene();
		ID3D11Device *GetDevice() const;
		ID3D11DeviceContext *GetDeviceContext() const;
		const CMatrix4x4 &GetProjectionMatrix() const;
		const CMatrix4x4 &GetWorldMatrix() const;
		bool Init(HWND hWnd, uint32 screenWidth, uint32 screenHeight, bool useVSync, bool fullscreen, float screenDepth, float screenNear);
		void Release();
	};
}