//Class header
#include "../../headers/Rendering/IRenderer.h"
//Local
#include "Default/CDefaultRenderer.h"
#include "Raytracer/CRayTraceRenderer.h"
//Namespaces
using namespace ACGE;

//Global functions
IRenderer *ACGE::CreateRenderer(ERenderer renderer, ACStdLib::Rendering::CDeviceContext &refDeviceContext)
{
	switch(renderer)
	{
	case ERenderer::Default:
		return new CDefaultRenderer(refDeviceContext);
	case ERenderer::RayTracer:
		return new CRayTraceRenderer(refDeviceContext);
	}

	return nullptr;
}