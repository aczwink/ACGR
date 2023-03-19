#pragma once
#include <ACStdLib.h>
//Local
#include "../Definitions.h"

namespace ACGE
{
	//Forward declarations
	class CCamera;
	class CLight;
	class CMaterial;
	class CMesh;
	class CSceneManager;
	class CSkyBox;

	enum class ERenderer
	{
		Default,
		RayTracer
	};

	class ACGE_API IRenderer
	{
	public:
		//Destructor
		virtual ~IRenderer() {}
		
		//Abstract
		virtual void DeviceStateChanged(const ACStdLib::CSize &refSize) = NULL;
		virtual void EnableDebugMode(bool state) = NULL;
		virtual void RenderFrame(const CSceneManager &refSceneMgr, const CCamera &refCamera) = NULL;
	};
	
	//Functions
	ACGE_API IRenderer *CreateRenderer(ERenderer renderer, ACStdLib::Rendering::CDeviceContext &refDeviceContext);
}