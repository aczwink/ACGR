#pragma once
//Global
#include <Windows.h>
//Local
#include "CGraphics.h"
#include "Definitions.h"

namespace CurseGine
{
	class CURSEGINE_API CEngine
	{
	private:
		//Variables
		HINSTANCE hInstance;
		//Constructor
		CEngine();
		//Functions
		void Render();
	public:
		//Variables
		CGraphics graphics;
		//Functions
		static CEngine &Get();
		bool Init(SJCLib::CString windowTitle, bool fullscreen, bool useVSync, HINSTANCE hInstance, int nCmdShow);
		void Run();
		void Shutdown();
	};
}