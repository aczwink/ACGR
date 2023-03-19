//Class header
#include "CShadowPass.h"
//Local
#include "../CDefaultRenderer.h"
//Namespaces
using namespace ACGE;

//Constructor
CShadowPass::CShadowPass(CDefaultRenderer &refRenderer) : refRenderer(refRenderer)
{
	this->pShadowProgram = this->refRenderer.GetShaderCompiler().FetchStaticProgram("Shadow");
	
	this->pFrameBuffer = this->refRenderer.GetDeviceContext().CreateFrameBuffer();

	this->pFrameBuffer->SetColorBuffer(nullptr);
}

//Destructor
CShadowPass::~CShadowPass()
{
	delete this->pFrameBuffer;
}

//Private methods
void CShadowPass::RenderNode(const CSceneNode &refNode, const CMatrix4x4 &refM)
{
	CMatrix4x4 transformation;

	transformation = refNode.GetTransformation() * refM;

	//render entities
	for(CEntity *const& refpEntity : refNode.GetAttachedEntities())
	{
		this->pShadowProgram->SetUniformValue(1, transformation);
		
		this->refRenderer.RenderMesh(refpEntity->GetMesh());
	}

	//render children
	for(CSceneNode *const& refpChild : refNode.GetChildren())
	{
		this->RenderNode(*refpChild, transformation);
	}
}

//Public methods
void CShadowPass::RenderDepthMap(SLightInfo &refLightInfo, const CSceneNode &refNode, const CMatrix4x4 &refM)
{
	CDeviceContext &refDC = this->refRenderer.GetDeviceContext();
	
	refDC.SetProgram(this->pShadowProgram);
	this->pShadowProgram->SetUniformValue(0, refLightInfo.lightVP);
	
	refDC.SetFrameBuffer(this->pFrameBuffer);
	this->pFrameBuffer->SetDepthBuffer(refLightInfo.pShadowMap);
	
	refDC.ClearDepthBuffer();
	
	this->RenderNode(refNode, refM);
}