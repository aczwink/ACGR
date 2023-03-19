//Class Header
#include "CEngine.h"
//SJCLib
#include <SJCLib.h>
//Local
#include "CLog.h"
#include "resource.h"
//Namespaces
using namespace SJCLib;
using namespace CurseGine;
//Definitions
#define CURSEGINE_WNDCLASS "CurseGineWndClass"

//Internal Functions
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(EXIT_SUCCESS);
		return 0;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//Constructor
CEngine::CEngine()
{
}

//Private Functions
void CEngine::Render()
{
	this->graphics.Render();
}

//Public Functions
CEngine &CEngine::Get()
{
	static CEngine engine;
	return engine;
}

bool CEngine::Init(CString windowTitle, bool fullscreen, bool useVSync, HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASS wc;
	HWND hWnd;
	uint32 posX, posY, screenWidth, screenHeight;

#ifdef _DEBUG
	CLog::Init();
#endif

	this->hInstance = hInstance;

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = CURSEGINE_WNDCLASS;
	wc.lpszMenuName = NULL;
	wc.style = 0;

	if(!RegisterClass(&wc))
		return false;

	if(fullscreen)
	{
		posX = 0;
		posY = 0;
		screenWidth = GetSystemMetrics(SM_CXSCREEN);
		screenHeight = GetSystemMetrics(SM_CYSCREEN);
	}
	else
	{
		screenWidth = 800;
		screenHeight = 600;
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	hWnd = CreateWindow(CURSEGINE_WNDCLASS, windowTitle.GetC_Str(), WS_OVERLAPPEDWINDOW, posX, posY, screenWidth, screenHeight, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	
	if(!this->graphics.Init(hWnd, screenWidth, screenHeight, fullscreen, useVSync))
		return false;

	return true;
}

void CEngine::Run()
{
	MSG msg;

	while(true)
	{
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if(msg.message == WM_QUIT)
			break;
		else
			this->Render();
	}
}

void CEngine::Shutdown()
{
	UnregisterClass(CURSEGINE_WNDCLASS, this->hInstance);
	this->graphics.Release();
}