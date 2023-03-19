#pragma once
//Global
#include <Windows.h>
//SJCLib
#include <SJCLib.h>
//Local
#include "CCamera.h"
#include "CD3D.h"
#include "CModel.h"
#include "CShader.h"
#include "Definitions.h"

namespace CurseGine
{
	class CURSEGINE_API CGraphics
	{
	private:
		//Variables
		CModel model;
		CShader shader;
	public:
		//Variables
		CD3D d3d;
		CCamera camera;
		//Functions
		bool Init(HWND hWnd, uint32 screenWidth, uint32 screenHeight, bool fullscreen, bool useVSync);
		void Release();
		void Render();
	};
}