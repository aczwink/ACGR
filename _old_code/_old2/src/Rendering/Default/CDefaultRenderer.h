#pragma once
//Local
#include "../../../headers/Rendering/IRenderer.h"
#include "../../../headers/Resources/CTexture.h"
#include "../../../headers/Scene/CSceneNode.h"
#include "../../../headers/Scene/CSkyBox.h"
#include "Passes/CShadowPass.h"
#include "../../Internal/CShaderCompiler.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Math;
using namespace ACStdLib::Rendering;

namespace ACGE
{
	struct SLightInfo
	{
		CMatrix4x4 lightVP;
		ITexture2D *pShadowMap;
	};

	class ACGE_API CDefaultRenderer : public IRenderer
	{
		struct SMeshObjects
		{
			IInputState *pInputState;
			IVertexBuffer *pVertexBuffer;
			IIndexBuffer *pIndexBuffer;
		};
	private:
		//Members
		bool debugMode;
		CDeviceContext &refDeviceContext;
		CSize deviceSize;
		CMatrix4x4 view;
		CMatrix4x4 projection;
		CMatrix4x4 VP;
		CShaderCompiler shaderCompiler;
		struct
		{
			IShaderProgram *pNormalsProgram;
		} debug;
		uint32 nLights;
		IShaderProgram *pMeshProgram;
		CInputLayout meshInputLayout;
		CMap<const CMesh *, SMeshObjects> meshObjects;
		CMap<const CTexture *, ITexture *> textures;
		struct
		{
			IShaderProgram *pFrameProgram;
			IFrameBuffer *pFrameBuffer;
			ITexture2D *pFrameColorBuffer;
			IInputState *pPlaneInputState;
			IVertexBuffer *pPlaneVertexBuffer;
		};
		struct
		{
			CSkyBox textures;
			IShaderProgram *pProgram;
			IInputState *pBoxInputState;
			IVertexBuffer *pBoxVertexBuffer;
			ICubeMap *pCubeMap;
		} skyBox;
		
		CShadowPass shadowPass;
		CMap<const CLight *, SLightInfo> additionalLightInfo;
		
		//Methods
		void BeginRendering(const CSceneManager &refSceneMgr, const CCamera &refCamera);
		void EnableLight(const CLight *pLight, bool state = true);
		void EndRendering();
		void InitPrograms();
		void Render(const CMesh *pMesh, const CMatrix4x4 &refM);
		void Render(const CSceneNode &refNode, const CMatrix4x4 &refM);
		void RenderShadowMap(const CLight *pLight, const CSceneNode &refNode, const CMatrix4x4 &refM);
		void SetMaterial(const CMaterial *pMaterial);
		void SetSkyBox(const CSkyBox &refSkyBox);
		void SetTexture(const CTexture *pTexture);
		void SetupFrameBuffer();
		void SetupMeshObjects(const CMesh *pMesh, SMeshObjects &refMeshObjects);
		void SetupSkyBox();

	public:
		//Constructor
		CDefaultRenderer(CDeviceContext &refDeviceContext);

		//Destructor
		~CDefaultRenderer();

		//Methods
		void DeviceStateChanged(const CSize &refSize);
		void EnableDebugMode(bool state);
		void RenderFrame(const CSceneManager &refSceneMgr, const CCamera &refCamera);
		void RenderMesh(const CMesh *pMesh);

		//Inline
		inline CDeviceContext &GetDeviceContext()
		{
			return this->refDeviceContext;
		}

		inline CShaderCompiler &GetShaderCompiler()
		{
			return this->shaderCompiler;
		}
		
		inline void Render(const CEntity &refEntity, const CMatrix4x4 &refM)
		{
			if(refEntity.GetMaterial())
				this->SetMaterial(refEntity.GetMaterial());
			else
				this->SetMaterial(refEntity.GetMesh()->GetMaterial());

			this->Render(refEntity.GetMesh(), refM);
		}
	};
}