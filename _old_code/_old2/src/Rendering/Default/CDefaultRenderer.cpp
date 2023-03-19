//Class header
#include "CDefaultRenderer.h"
//Local
#include "../../../headers/Resources/CMaterial.h"
#include "../../../headers/Resources/CMesh.h"
#include "../../../headers/Scene/CCamera.h"
#include "../../../headers/Scene/CLight.h"
#include "../../../headers/Scene/CSceneManager.h"
//Namespaces
using namespace ACGE;

//Constants
const uint32 MAX_LIGHTS = 4;

enum EMeshUniformIds
{
	UNIFORM_ID_MESH_MODEL,
	UNIFORM_ID_MESH_MVP,
	UNIFORM_ID_MESH_AMBIENT_LIGHT,
	UNIFORM_ID_MESH_CAMERA_POS,
	UNIFORM_ID_MESH_ENVIRONMENT,
	UNIFORM_ID_MESH_MAX_LIGHT_INDEX
};

enum ETextureUnitAllocation
{
	TEXTURE_UNIT_MATERIAL,
	TEXTURE_UNIT_ENVIRONMENT,

	//let the shadow maps be the last ones
	TEXTURE_UNIT_SHADOWMAP_0,
};

//Constructor
CDefaultRenderer::CDefaultRenderer(CDeviceContext &refDeviceContext) : refDeviceContext(refDeviceContext), shaderCompiler(refDeviceContext), shadowPass(*this)
{
	this->debugMode = false;
	this->nLights = 0;

	this->InitPrograms();
	this->SetupFrameBuffer();
	this->SetupSkyBox();
}

//Destructor
CDefaultRenderer::~CDefaultRenderer()
{
	for(auto &refKV : this->meshObjects)
	{
		delete refKV.value.pInputState;
		delete refKV.value.pIndexBuffer;
		delete refKV.value.pVertexBuffer;
	}

	for(auto &refKV : this->textures)
		delete refKV.value;

	//release frame buffer
	delete this->pPlaneInputState;
	delete this->pPlaneVertexBuffer;
	
	delete this->pFrameColorBuffer;
	delete this->pFrameBuffer;
	
	//release sky box
	delete this->skyBox.pBoxInputState;
	delete this->skyBox.pBoxVertexBuffer;
	delete this->skyBox.pCubeMap;

	//release shadow maps
	for(auto &refKV : this->additionalLightInfo)
		delete refKV.value.pShadowMap;
}

//Private methods
void CDefaultRenderer::BeginRendering(const CSceneManager &refSceneMgr, const CCamera &refCamera)
{
	//camera
	this->view = refCamera.GetViewMatrix();
	this->projection = refCamera.GetPerspectiveMatrix();
	this->VP = this->projection * this->view;
	
	this->pMeshProgram->SetUniformValue(UNIFORM_ID_MESH_CAMERA_POS, refCamera.position);

	//set the sky box
	this->SetSkyBox(refSceneMgr.GetSkyBox());
	this->refDeviceContext.SetTexture(TEXTURE_UNIT_ENVIRONMENT, this->skyBox.pCubeMap);
	this->pMeshProgram->SetUniformValue(UNIFORM_ID_MESH_ENVIRONMENT, (int32)TEXTURE_UNIT_ENVIRONMENT);

	//set uniforms that don't change for the entire frame
	this->pMeshProgram->SetUniformValue(UNIFORM_ID_MESH_AMBIENT_LIGHT, refSceneMgr.ambientLight);

	this->pMeshProgram->SetUniformValue(this->pMeshProgram->GetUniformId("material.tex"), (int32)TEXTURE_UNIT_MATERIAL);
}

void CDefaultRenderer::EnableLight(const CLight *pLight, bool state)
{
	if(state)
	{
		if(this->nLights < MAX_LIGHTS)
		{
			C8BitString prefix;
			
			SLightInfo &refLightInfo = this->additionalLightInfo[pLight];

			//set the light in shader
			prefix = "lights[" + ToString_8Bit(this->nLights) + "].";
			this->pMeshProgram->SetUniformValue(this->pMeshProgram->GetUniformId(prefix + "type"), (uint32)pLight->type);
			this->pMeshProgram->SetUniformValue(this->pMeshProgram->GetUniformId(prefix + "color"), pLight->color);
			this->pMeshProgram->SetUniformValue(this->pMeshProgram->GetUniformId(prefix + "power"), pLight->power);
			this->pMeshProgram->SetUniformValue(this->pMeshProgram->GetUniformId(prefix + "lightVP"), refLightInfo.lightVP);
			this->pMeshProgram->SetUniformValue(this->pMeshProgram->GetUniformId(prefix + "direction"), pLight->direction);
			this->pMeshProgram->SetUniformValue(this->pMeshProgram->GetUniformId(prefix + "pos"), pLight->position);
			this->pMeshProgram->SetUniformValue(this->pMeshProgram->GetUniformId(prefix + "innerConeAngle"), (float32)cos(CRadian(pLight->innerConeAngle).value));
			this->pMeshProgram->SetUniformValue(this->pMeshProgram->GetUniformId(prefix + "openingAngle"), (float32)cos(CRadian(pLight->openingAngle).value));

			//update number of lights in shader
			this->pMeshProgram->SetUniformValue(UNIFORM_ID_MESH_MAX_LIGHT_INDEX, this->nLights + 1);

			//set shadow map
			this->refDeviceContext.SetTexture(uint8(TEXTURE_UNIT_SHADOWMAP_0 + this->nLights), refLightInfo.pShadowMap);
			this->pMeshProgram->SetUniformValue(this->pMeshProgram->GetUniformId(prefix + "shadowMap"), int32(TEXTURE_UNIT_SHADOWMAP_0 + this->nLights));
		}

		this->nLights++;
	}
	else
	{
		this->nLights--;
		if(this->nLights < MAX_LIGHTS)
		{
			//update number of lights in shader
			this->pMeshProgram->SetUniformValue(UNIFORM_ID_MESH_MAX_LIGHT_INDEX, this->nLights);
		}
	}
}

void CDefaultRenderer::EndRendering()
{
	//render sky box
	this->refDeviceContext.SetProgram(this->skyBox.pProgram);
	this->refDeviceContext.SetInputState(this->skyBox.pBoxInputState);
	this->skyBox.pProgram->SetUniformValue(this->skyBox.pProgram->GetUniformId("transform"), this->projection * CMatrix4x4(CMatrix3x3(this->view)));

	this->refDeviceContext.SetDepthTest(ETestFunction::LessOrEqual);
	this->refDeviceContext.DrawTriangles(0, 12);
	this->refDeviceContext.SetDepthTest(ETestFunction::Less);

	/*
	//reset to screen frame buffer
	this->refDeviceContext.SetFrameBuffer(nullptr);
	this->refDeviceContext.SetProgram(this->pFrameProgram);
	this->refDeviceContext.SetTexture(0, this->pFrameColorBuffer);

	this->refDeviceContext.SetInputState(this->pPlaneInputState);
	this->refDeviceContext.DrawTriangles(0, 2);
	*/
}

void CDefaultRenderer::InitPrograms()
{
	this->pFrameProgram = this->shaderCompiler.FetchStaticProgram("Frame");
	this->pMeshProgram = this->shaderCompiler.FetchStaticProgram("Mesh");
	this->debug.pNormalsProgram = this->shaderCompiler.FetchStaticProgram("Normals");

	this->meshInputLayout.AddAttribute3(); //pos
	this->meshInputLayout.AddAttribute3(); //normal
	this->meshInputLayout.AddAttribute2(); //texcoords

	//init shader uniforms
	this->pMeshProgram->SetUniformValue(UNIFORM_ID_MESH_MAX_LIGHT_INDEX, 0u);
}

void CDefaultRenderer::Render(const CMesh *pMesh, const CMatrix4x4 &refM)
{
	//program
	this->refDeviceContext.SetProgram(this->pMeshProgram);
	this->pMeshProgram->SetUniformValue(UNIFORM_ID_MESH_MODEL, refM);
	this->pMeshProgram->SetUniformValue(UNIFORM_ID_MESH_MVP, this->VP * refM);
	
	//render mesh
	this->RenderMesh(pMesh);
	
	if(this->debugMode)
	{
		this->debug.pNormalsProgram->SetUniformValue(UNIFORM_ID_MESH_MODEL, refM);
		this->debug.pNormalsProgram->SetUniformValue(UNIFORM_ID_MESH_MVP, this->VP * refM);
		this->debug.pNormalsProgram->SetUniformValue(this->debug.pNormalsProgram->GetUniformId("view"), this->view);
		this->debug.pNormalsProgram->SetUniformValue(this->debug.pNormalsProgram->GetUniformId("projection"), this->projection);

		this->refDeviceContext.SetProgram(this->debug.pNormalsProgram);
		this->refDeviceContext.DrawTrianglesIndexed();
	}
}

void CDefaultRenderer::Render(const CSceneNode &refNode, const CMatrix4x4 &refM)
{
	CMatrix4x4 transformation;

	transformation = refNode.GetTransformation() * refM;

	//enable lights
	for(CLight *const& refpLight : refNode.GetAttachedLights())
	{
		this->EnableLight(refpLight);
	}

	//render entities
	for(CEntity *const& refpEntity : refNode.GetAttachedEntities())
	{
		this->Render(*refpEntity, transformation);
	}

	//render children
	for(CSceneNode *const& refpChild : refNode.GetChildren())
	{
		this->Render(*refpChild, transformation);
	}

	//disable lights
	for(CLight *const& refpLight : refNode.GetAttachedLights())
	{
		this->EnableLight(refpLight, false);
	}
}

void CDefaultRenderer::RenderShadowMap(const CLight *pLight, const CSceneNode &refNode, const CMatrix4x4 &refM)
{
	CCamera lightCam;

	if(!this->additionalLightInfo.Contains(pLight))
	{
		SLightInfo lightInfo;

		//shadow map
		lightInfo.pShadowMap = this->refDeviceContext.CreateTexture2D();
		lightInfo.pShadowMap->AllocateDepth(this->deviceSize.width, this->deviceSize.height);
		lightInfo.pShadowMap->SetMaximumMipMapLevel(0);
		
		this->additionalLightInfo[pLight] = lightInfo;
	}
	
	SLightInfo &refLightInfo = this->additionalLightInfo[pLight];

	//lightVP
	lightCam.position = pLight->position;
	lightCam.SetViewDirection(pLight->direction);
	refLightInfo.lightVP = CMatrix4x4::OrthographicRH(-10, 10, -10, 10, 0.1f, 1000.0f) * lightCam.GetViewMatrix();
	
	this->shadowPass.RenderDepthMap(refLightInfo, refNode, refM);
}

void CDefaultRenderer::SetMaterial(const CMaterial *pMaterial)
{
	if(!pMaterial)
	{
		//no material
		this->refDeviceContext.EnableBlending(false); //render opaque
		this->SetTexture(nullptr);
		
		return;
	}


	this->refDeviceContext.EnableBlending(pMaterial->useAlphaBlending);
	this->SetTexture(pMaterial->GetTexture());
}

void CDefaultRenderer::SetSkyBox(const CSkyBox &refSkyBox)
{
	if(this->skyBox.textures.pBack != refSkyBox.pBack)
	{
		this->skyBox.pCubeMap->SetBackTexture(refSkyBox.pBack->GetWidth(), refSkyBox.pBack->GetHeight(), refSkyBox.pBack->GetData());
		this->skyBox.textures.pBack = refSkyBox.pBack;
	}
	if(this->skyBox.textures.pBottom != refSkyBox.pBottom)
	{
		this->skyBox.pCubeMap->SetBottomTexture(refSkyBox.pBottom->GetWidth(), refSkyBox.pBottom->GetHeight(), refSkyBox.pBottom->GetData());
		this->skyBox.textures.pBottom = refSkyBox.pBottom;
	}
	if(this->skyBox.textures.pFront != refSkyBox.pFront)
	{
		this->skyBox.pCubeMap->SetFrontTexture(refSkyBox.pFront->GetWidth(), refSkyBox.pFront->GetHeight(), refSkyBox.pFront->GetData());
		this->skyBox.textures.pFront = refSkyBox.pFront;
	}
	if(this->skyBox.textures.pLeft != refSkyBox.pLeft)
	{
		this->skyBox.pCubeMap->SetLeftTexture(refSkyBox.pLeft->GetWidth(), refSkyBox.pLeft->GetHeight(), refSkyBox.pLeft->GetData());
		this->skyBox.textures.pLeft = refSkyBox.pLeft;
	}
	if(this->skyBox.textures.pRight != refSkyBox.pRight)
	{
		this->skyBox.pCubeMap->SetRightTexture(refSkyBox.pRight->GetWidth(), refSkyBox.pRight->GetHeight(), refSkyBox.pRight->GetData());
		this->skyBox.textures.pRight = refSkyBox.pRight;
	}
	if(this->skyBox.textures.pTop != refSkyBox.pTop)
	{
		this->skyBox.pCubeMap->SetTopTexture(refSkyBox.pTop->GetWidth(), refSkyBox.pTop->GetHeight(), refSkyBox.pTop->GetData());
		this->skyBox.textures.pTop = refSkyBox.pTop;
	}
}

void CDefaultRenderer::SetTexture(const CTexture *pTexture)
{
	if(!pTexture)
	{
		//this->refDeviceContext.SetTexture(TEXTURE_UNIT_MATERIAL, nullptr);
		return;
	}
	
	if(!this->textures.Contains(pTexture))
	{
		ITexture2D *pDeviceTexture;

		pDeviceTexture = this->refDeviceContext.CreateTexture2D();
		
		switch(pTexture->GetFormat())
		{
		case TEXTURE_FORMAT_DXT1:
			pDeviceTexture->AllocateDXT1(pTexture->GetWidth(), pTexture->GetHeight(), pTexture->GetData());
			break;
		case TEXTURE_FORMAT_DXT5:
			pDeviceTexture->AllocateDXT5(pTexture->GetWidth(), pTexture->GetHeight(), pTexture->GetData());
			break;
		default:
			NOT_IMPLEMENTED_ERROR;
		}
		
		pDeviceTexture->GenerateMipMaps();
		
		this->textures[pTexture] = pDeviceTexture;
	}

	ITexture *const& refpDeviceTexture = this->textures[pTexture];

	this->refDeviceContext.SetTexture(TEXTURE_UNIT_MATERIAL, refpDeviceTexture);
}

void CDefaultRenderer::SetupMeshObjects(const CMesh *pMesh, SMeshObjects &refMeshObjects)
{
	//vertex buffer
	refMeshObjects.pVertexBuffer = this->refDeviceContext.CreateVertexBuffer();

	refMeshObjects.pVertexBuffer->Allocate(pMesh->GetNumberOfVertices(), sizeof(CMesh::SVertex), pMesh->GetVertices());

	//index buffer
	refMeshObjects.pIndexBuffer = this->refDeviceContext.CreateIndexBuffer();

	if(pMesh->GetNumberOfVertices() > UINT16_MAX)
		refMeshObjects.pIndexBuffer->Allocate(pMesh->GetNumberOfIndices(), pMesh->GetIndices32());
	else
		refMeshObjects.pIndexBuffer->Allocate(pMesh->GetNumberOfIndices(), pMesh->GetIndices16());

	//input state
	refMeshObjects.pInputState = this->refDeviceContext.CreateInputState();
	refMeshObjects.pInputState->AddVertexBuffer(refMeshObjects.pVertexBuffer, this->meshInputLayout);
	refMeshObjects.pInputState->SetIndexBuffer(refMeshObjects.pIndexBuffer);
}

void CDefaultRenderer::SetupFrameBuffer()
{
	CSize screenSize;
	CInputLayout inputLayout;

	static const float32 planePositions[][2] =
	{
		//lower left
		{-1, 1}, //top
		{-1, -1}, //bottom
		{1, -1}, //right

		//upper right
		{1, 1}, //top
		{-1, 1}, //left
		{1, -1}, //bottom
	};

	//set up color buffer for frame buffer
	screenSize = CSize::GetScreenSize();

	this->pFrameColorBuffer = this->refDeviceContext.CreateTexture2D();
	this->pFrameColorBuffer->AllocateRGB(screenSize.width, screenSize.height, nullptr);
	this->pFrameColorBuffer->SetMaximumMipMapLevel(0);

	//set up frame buffer
	this->pFrameBuffer = this->refDeviceContext.CreateFrameBuffer();
	this->pFrameBuffer->SetColorBuffer(this->pFrameColorBuffer);

	stdOut << this->pFrameBuffer->GetStatus();

	//create input layout
	inputLayout.AddAttribute2();
	
	//plane vertex buffer
	this->pPlaneVertexBuffer = this->refDeviceContext.CreateVertexBuffer();
	this->pPlaneVertexBuffer->Allocate(6, sizeof(planePositions[0]), planePositions);

	//plane input state
	this->pPlaneInputState = this->refDeviceContext.CreateInputState();
	this->pPlaneInputState->AddVertexBuffer(this->pPlaneVertexBuffer, inputLayout);
}

void CDefaultRenderer::SetupSkyBox()
{
	CInputLayout inputLayout;

	static const float32 skyboxPositions[] =
	{
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		
		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
		
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	//create program
	this->skyBox.pProgram = this->shaderCompiler.FetchStaticProgram("SkyBox");

	this->skyBox.pProgram->SetUniformValue(this->skyBox.pProgram->GetUniformId("skyBoxTexture"), (int32)TEXTURE_UNIT_ENVIRONMENT);

	//create input layout
	inputLayout.AddAttribute3(); //position
	
	//box vertex buffer
	this->skyBox.pBoxVertexBuffer = this->refDeviceContext.CreateVertexBuffer();
	this->skyBox.pBoxVertexBuffer->Allocate(36, sizeof(float32)*3, skyboxPositions);

	//plane input state
	this->skyBox.pBoxInputState = this->refDeviceContext.CreateInputState();
	this->skyBox.pBoxInputState->AddVertexBuffer(this->skyBox.pBoxVertexBuffer, inputLayout);

	//create cube map
	this->skyBox.pCubeMap = refDeviceContext.CreateCubeMap();
}

//Public methods
void CDefaultRenderer::DeviceStateChanged(const CSize &refSize)
{
	this->deviceSize = refSize;

	//resize all shadow depth maps
	for(auto &refKV : this->additionalLightInfo)
		refKV.value.pShadowMap->AllocateDepth(refSize.width, refSize.height);
	
	//this->pFrameColorBuffer->Allocate(false, refSize.width, refSize.height, nullptr);
}

void CDefaultRenderer::EnableDebugMode(bool state)
{
	this->debugMode = state;
}

void CDefaultRenderer::RenderFrame(const CSceneManager &refSceneMgr, const CCamera &refCamera)
{
	//set device settings
	this->refDeviceContext.EnableDepthTest();
	this->refDeviceContext.EnableFaceCulling();

	//first do shadow passes for all lights
	auto lights = refSceneMgr.GetLights();
	for(const auto &refKV : lights)
		this->RenderShadowMap(refKV.key, *refKV.value, refKV.value->ComputeModelMatrix());
	
	//SHOW SHADOW MAP
	/*
	this->refDeviceContext.SetFrameBuffer(nullptr);
	this->refDeviceContext.ClearColorBuffer(CColor(0.1f, 0.5f, 0.1f, 1));
	this->refDeviceContext.ClearDepthBuffer();
	this->refDeviceContext.SetProgram(this->pFrameProgram);
	this->refDeviceContext.SetTexture(0, (*this->additionalLightInfo.begin()).value.pShadowMap);
	
	this->refDeviceContext.SetInputState(this->pPlaneInputState);
	this->refDeviceContext.DrawTriangles(0, 2);
	return;
	*/
	//SHOW SHADOW MAP

	//now render scene
	this->BeginRendering(refSceneMgr, refCamera);

	this->refDeviceContext.SetFrameBuffer(nullptr); //switch to screen frame buffer

	this->refDeviceContext.ClearColorBuffer(CColor(0.1f, 0.1f, 0.1f, 1));
	this->refDeviceContext.ClearDepthBuffer();

	this->Render(*refSceneMgr.GetRootNode(), CMatrix4x4::Identity());

	this->EndRendering();
}

void CDefaultRenderer::RenderMesh(const CMesh *pMesh)
{
	if(!this->meshObjects.Contains(pMesh))
		this->SetupMeshObjects(pMesh, this->meshObjects[pMesh]);
	
	SMeshObjects &refMeshObj = this->meshObjects[pMesh];
	
	this->refDeviceContext.SetInputState(refMeshObj.pInputState);

	this->refDeviceContext.DrawTrianglesIndexed();
}