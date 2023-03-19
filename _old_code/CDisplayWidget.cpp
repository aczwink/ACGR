//Class header
#include "CDisplayWidget.h"
//Namespaces
using namespace ACStdLib::Math;

//Constructor
CDisplayWidget::CDisplayWidget(AWidgetContainer *pParent) : C3DView(pParent)
{
	this->haveRayTraceImage = false;
	
	this->pRenderer = CreateRenderer(ERenderer::Default, *this->pDeviceContext);
	this->pRayTracer = CreateRenderer(ERenderer::RayTracer, *this->pDeviceContext);
	
	this->InitScene();

	this->tLastFrame = this->GetTime();
	this->deltaTime = 0;
}

//Destructor
CDisplayWidget::~CDisplayWidget()
{
	delete this->pRenderer;
	delete this->pRayTracer;
}

//Eventhandlers
void CDisplayWidget::OnKeyReleased(const Events::CKeyEvent &refKeyEvent)
{
	switch(refKeyEvent.GetKeyCode())
	{
	case EKeyCode::Space:
		{
			this->pCamera->EnableFreeLook(!this->pCamera->IsFreeLookEnabled());
		}
		break;
	}
}

void CDisplayWidget::OnPaint()
{
	this->UpdateScene();
	
	if(!this->haveRayTraceImage)
	{
		this->pRenderer->RenderFrame(this->sceneMgr, *this->pCamera);

		this->pDeviceContext->SwapBuffers();
	}
}

void CDisplayWidget::OnResized()
{
	C3DView::OnResized();

	this->haveRayTraceImage = false;

	this->pRenderer->DeviceStateChanged(this->GetSize());
	this->pRayTracer->DeviceStateChanged(this->GetSize());

	this->pCamera->SetAspectRatio(this->GetSize().width / (float32)this->GetSize().height);

	this->Repaint();
}

//Private methods
float32 CDisplayWidget::GetTime()
{
	static uint64 referenceCounter = QueryHighPrecisionClockCounter();

	return float32(float64(QueryHighPrecisionClockCounter() - referenceCounter) / (float64)QueryHighPrecisionClockFrequency());
}

void CDisplayWidget::InitScene()
{
	CResourceGroup *pResourceGroup;
	
	//Load resources
	pResourceGroup = new CResourceGroup;
	
	pResourceGroup->AddPrimitives();
	pResourceGroup->AddLocation(CPath("data"), true);
	//pResourceGroup->AddLocation(CPath("fl_data"), true);

	CResourcesManager::GetInstance().AppendResourceGroup(pResourceGroup);
	
	//Load scene
	this->sceneMgr.Load(CFileInputStream(CPath("scenes/cornell_box.xml")));
	
	//find camera
	this->pCamera = (CFreeFlyCamera *)this->sceneMgr.GetRootNode()->GetAttachedCameras()[0];
}

void CDisplayWidget::UpdateScene()
{
	float32 currentTime;
	
	//delta time
	currentTime = this->GetTime();
	this->deltaTime = currentTime - this->tLastFrame;
	this->tLastFrame = currentTime;

	//update camera
	this->pCamera->UpdateCursorPos(this->deltaTime);
}