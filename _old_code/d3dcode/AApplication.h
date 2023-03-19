//Global
#include <Windows.h>
//Libs
#include <ACStdLib.h>
//Local
#include "CD3D.h"
#include "CShaderManager.h"
#include "Definitions.h"
//Definitions
#undef CreateWindow

namespace ACGE
{
	class ACGE_API AApplication
	{
	private:
		//Members
		HINSTANCE hInstance;
		HWND hWnd;
		//Abstract
		virtual void RenderScene() = NULL;
		virtual void UpdateScene() = NULL;
		//Callbacks
		LRESULT CALLBACK OnWindowsMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		//Functions
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		//Methods
		bool CreateWindow(const ACStdLib::CString &refWindowTitle, int nCmdShow);
		bool RegisterWindowClass();
	protected:
		//Members
		CD3D d3d;
		CShaderManager shaderManager;
	public:
		//Constructor
		AApplication();
		//Destructor
		~AApplication();
		//Methods
		bool Init(HINSTANCE hInstance, const ACStdLib::CString &refWindowTitle, int nCmdShow);
		int32 Run();
	};
}