/*
//Local
#include "../../headers/Rendering/IRenderer.h"
#include "../../headers/Resources/CResourcesManager.h"

using namespace ACStdLib::Math;


//Public methods
void CMesh::Render() const
{
	IRenderer &refRenderer = IRenderer::GetInstance();
	
	refRenderer.SetRenderMode(RENDERMODE_MESH);
	refRenderer.SetMaterial(this->pMaterial);
	this->pRenderState->DrawIndexed();
}
*/