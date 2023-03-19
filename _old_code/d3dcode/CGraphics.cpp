//Class Header
#include "CGraphics.h"
//Local
#include "CLog.h"
//Namespaces
using namespace CurseGine;
//Definitions
#define SCREEN_DEPTH 1000.0f
#define SCREEN_NEAR 0.1f

//Public Functions
bool CGraphics::Init(HWND hWnd, uint32 screenWidth, uint32 screenHeight, bool fullscreen, bool useVSync)
{
	if(!this->d3d.Init(hWnd, screenWidth, screenHeight, useVSync, fullscreen, SCREEN_DEPTH, SCREEN_NEAR))
		return false;

	this->camera.SetPosition(CVector3(0, 0, -10));

	if(!this->shader.Init("../Shaders/Color.ps", "../Shaders/Color.vs"))
	{
		LOGERROR("Failed to initiate shaders.");
	}
	this->model.Init();

	return true;
}

void CGraphics::Release()
{
	this->model.Release();
	this->shader.Release();
	this->d3d.Release();
}

void CGraphics::Render()
{
	this->d3d.BeginScene(CColor(0, 0.125f, 0.3f, 1.0));
	this->camera.Update();
	
	this->model.Render();
	this->shader.Render(3, this->d3d.GetWorldMatrix(), this->camera.GetViewMatrix(), this->d3d.GetProjectionMatrix());
	
	
	this->d3d.EndScene();
}