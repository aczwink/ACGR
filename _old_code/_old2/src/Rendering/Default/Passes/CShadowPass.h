#pragma once
#include <ACStdLib.h>
//Local
#include "../../../../headers/Scene/CSceneNode.h"
#include "../../../Internal/CShaderCompiler.h"

//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Math;
using namespace ACStdLib::Rendering;

namespace ACGE
{
	//Forward declarations
	class CDefaultRenderer;
	struct SLightInfo;

	class CShadowPass
	{
	private:
		//Members
		IShaderProgram *pShadowProgram;
		IFrameBuffer *pFrameBuffer;
		
		CDefaultRenderer &refRenderer;

		//Methods
		void RenderNode(const CSceneNode &refNode, const CMatrix4x4 &refM);

	public:
		//Constructor
		CShadowPass(CDefaultRenderer &refRenderer);

		//Destructor
		~CShadowPass();

		//Methods
		void RenderDepthMap(SLightInfo &refLightInfo, const CSceneNode &refNode, const CMatrix4x4 &refM);
	};
}