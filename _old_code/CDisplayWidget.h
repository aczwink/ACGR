#pragma once
#include <ACStdLib.h>
using namespace ACStdLib;
using namespace ACStdLib::UI;
using namespace ACStdLib::Rendering;
//Libs
#include <ACGE.h>
//Namespaces
using namespace ACGE;

class CDisplayWidget : public C3DView
{
private:
	//Members
	bool haveRayTraceImage;
	IRenderer *pRenderer;
	IRenderer *pRayTracer;
	CSceneManager sceneMgr;
	CFreeFlyCamera *pCamera;
	float32 tLastFrame;
	float32 deltaTime;

	CSceneNode *pNode;
	
	//Eventhandlers
	void OnKeyReleased(const Events::CKeyEvent &refKeyEvent);
	void OnPaint();
	void OnResized();
	
	//Methods
	float32 GetTime();
	void InitScene();
	void UpdateScene();

public:
	//Constructor
	CDisplayWidget(AWidgetContainer *pParent);

	//Destructor
	~CDisplayWidget();

	//Inline
	inline void RayTrace()
	{
		this->pRayTracer->RenderFrame(this->sceneMgr, *this->pCamera);
		this->pDeviceContext->SwapBuffers();
		
		this->haveRayTraceImage = true;
	}
	
	inline void SetDebugMode(bool enabled)
	{
		this->pRenderer->EnableDebugMode(enabled);
	}
	
	inline void SetWireFrame(bool enabled)
	{
		this->pDeviceContext->EnablePolygonFilling(!enabled);
		this->Repaint();
	}
};
