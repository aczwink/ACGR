//Class Header
#include "AApplication.h"
//Local
#include "CConfig.h"
//Namespaces
using namespace ACStdLib;
using namespace ACGE;
//Definitions
#define ACGE_WINDOWCLASSNAME "ACGE3DWindow"

//Global Variables
AApplication *g_pApp = nullptr;

//Constructor
AApplication::AApplication() : shaderManager(this->d3d)
{
}

//Destructor
AApplication::~AApplication()
{
	UnregisterClass(ACGE_WINDOWCLASSNAME, this->hInstance);
}

//Private callbacks
LRESULT CALLBACK AApplication::OnWindowsMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(EXIT_SUCCESS);
			return 0;
		}
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

//Private functions
LRESULT CALLBACK AApplication::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return g_pApp->OnWindowsMessage(hWnd, message, wParam, lParam);
}

//Private methods
bool AApplication::CreateWindow(const CString &refWindowTitle, int nCmdShow)
{
	RECT rc;
	
	CConfig &refConfig = CConfig::Get();

	rc.left = (GetSystemMetrics(SM_CXSCREEN) - refConfig.GetResolutionWidth()) / 2;
	rc.top = (GetSystemMetrics(SM_CYSCREEN) - refConfig.GetResolutionHeight()) / 2;
	rc.right = rc.left + refConfig.GetResolutionWidth();
	rc.bottom = rc.top + refConfig.GetResolutionHeight();
	
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	
	this->hWnd = CreateWindowA(ACGE_WINDOWCLASSNAME, refWindowTitle.GetC_Str(), WS_OVERLAPPEDWINDOW, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, this->hInstance, NULL);
	if(!this->hWnd)
	{
		MessageBox(NULL, "Error creating window", "ACGE Init Error", MB_ICONERROR);
		return false;
	}
	
	ShowWindow(this->hWnd, nCmdShow);

	return true;
}

bool AApplication::RegisterWindowClass()
{
	WNDCLASS wc;
	
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = this->hInstance;
	wc.lpfnWndProc = AApplication::WndProc;
	wc.lpszClassName = ACGE_WINDOWCLASSNAME;
	wc.lpszMenuName = NULL;
	wc.style = 0;
	
	if(!RegisterClass(&wc))
	{
		MessageBox(NULL, "Error registering window class", "ACGE Init Error", MB_ICONERROR);
		return false;
	}

	return true;
}

//Public methods
bool AApplication::Init(HINSTANCE hInstance, const CString &refWindowTitle, int nCmdShow)
{
	this->hInstance = hInstance;
	
	g_pApp = this;
	
	if(!this->RegisterWindowClass())
		return false;
	if(!this->CreateWindow(refWindowTitle, nCmdShow))
		return false;

	//Renderer
	if(!this->d3d.Init(this->hWnd))
		return false;
	
	return true;
}

int32 AApplication::Run()
{
	MSG msg;

	while(true)
	{
		//Process all queued messages
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
				return (int32)msg.wParam;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		this->UpdateScene();
		this->RenderScene();
	}
	
	return EXIT_SUCCESS;
}