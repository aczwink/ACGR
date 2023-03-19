#pragma once
//Global
#include <d3d11.h>
//Local
#include "Definitions.h"

namespace ACGE
{
	class ACGE_API CD3D
	{
	private:
		//Members
		IDXGISwapChain *pSwapChain;
		ID3D11Device *pDevice;
		ID3D11DeviceContext *pImmediateContext;
		ID3D11RenderTargetView *pRenderTargetView;
		ID3D11Texture2D *pDepthStencilBuffer;
		ID3D11DepthStencilView *pDepthStencilView;
	public:
		//Constructor
		CD3D();
		//Destructor
		~CD3D();
		//Methods
		bool Init(HWND hWnd);
		//Inline
		inline ID3D11DepthStencilView *GetDepthStencilView() const
		{
			return this->pDepthStencilView;
		}
		
		inline ID3D11Device *GetDevice() const
		{
			return this->pDevice;
		}
		
		inline ID3D11DeviceContext *GetImmediateContext() const
		{
			return this->pImmediateContext;
		}

		inline ID3D11RenderTargetView *GetRenderTargetView() const
		{
			return this->pRenderTargetView;
		}

		inline IDXGISwapChain *GetSwapChain() const
		{
			return this->pSwapChain;
		}
	};
}