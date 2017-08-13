/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACGR.
 *
 * ACGR is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACGR is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACGR.  If not, see <http://www.gnu.org/licenses/>.
 */
//Class header
#include "DeviceRenderer.hpp"
//Local
#include <ACGR/Camera.hpp>
#include <ACGR/Material.hpp>
#include <ACGR/SceneManager.hpp>
//Namespaces
using namespace ACGR;

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
DeviceRenderer::DeviceRenderer(DeviceContext &refDeviceContext) : refDeviceContext(refDeviceContext), shaderCompiler(refDeviceContext), shadowPass(*this)
{
	this->debugMode = false;
	this->nLights = 0;

	this->InitPrograms();
	this->SetupFrameBuffer();
	this->SetupSkyBox();
}

//Destructor
DeviceRenderer::~DeviceRenderer()
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

//Public methods
void DeviceRenderer::RenderFrame(const SceneManager &refSceneMgr, const Camera &refCamera)
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

	this->refDeviceContext.ClearColorBuffer(Color(0.1f, 0.1f, 0.1f, 1));
	this->refDeviceContext.ClearDepthBuffer();

	this->Render(*refSceneMgr.GetRootNode(), Matrix4x4::Identity());

	this->EndRendering();
}

//Private methods
void DeviceRenderer::BeginRendering(const SceneManager &refSceneMgr, const Camera &refCamera)
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

void DeviceRenderer::EnableLight(const Light *pLight, bool state)
{
	if(state)
	{
		if(this->nLights < MAX_LIGHTS)
		{
			ByteString prefix;

			SLightInfo &refLightInfo = this->additionalLightInfo[pLight];

			//set the light in shader
			prefix = "lights[" + ToString_8Bit(this->nLights) + "].";
			this->pMeshProgram->SetUniformValue(this->pMeshProgram->GetUniformId(prefix + "type"), (uint32)pLight->type);
			this->pMeshProgram->SetUniformValue(this->pMeshProgram->GetUniformId(prefix + "color"), pLight->color);
			this->pMeshProgram->SetUniformValue(this->pMeshProgram->GetUniformId(prefix + "power"), pLight->power);
			this->pMeshProgram->SetUniformValue(this->pMeshProgram->GetUniformId(prefix + "lightVP"), refLightInfo.lightVP);
			this->pMeshProgram->SetUniformValue(this->pMeshProgram->GetUniformId(prefix + "direction"), pLight->direction);
			this->pMeshProgram->SetUniformValue(this->pMeshProgram->GetUniformId(prefix + "pos"), pLight->position);
			this->pMeshProgram->SetUniformValue(this->pMeshProgram->GetUniformId(prefix + "innerConeAngle"), (float32)cos(Radian(pLight->innerConeAngle).value));
			this->pMeshProgram->SetUniformValue(this->pMeshProgram->GetUniformId(prefix + "openingAngle"), (float32)cos(Radian(pLight->openingAngle).value));

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

void DeviceRenderer::EndRendering()
{
	//render sky box
	this->refDeviceContext.SetProgram(this->skyBox.pProgram);
	this->refDeviceContext.SetInputState(this->skyBox.pBoxInputState);
	this->skyBox.pProgram->SetUniformValue(this->skyBox.pProgram->GetUniformId("transform"), this->projection * Matrix4x4(Matrix3x3(this->view)));

	this->refDeviceContext.SetDepthTest(TestFunction::LessOrEqual);
	this->refDeviceContext.DrawTriangles(0, 12);
	this->refDeviceContext.SetDepthTest(TestFunction::Less);

	/*
	//reset to screen frame buffer
	this->refDeviceContext.SetFrameBuffer(nullptr);
	this->refDeviceContext.SetProgram(this->pFrameProgram);
	this->refDeviceContext.SetTexture(0, this->pFrameColorBuffer);

	this->refDeviceContext.SetInputState(this->pPlaneInputState);
	this->refDeviceContext.DrawTriangles(0, 2);
	*/
}

void DeviceRenderer::InitPrograms()
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

void DeviceRenderer::Render(const Mesh *pMesh, const Matrix4x4 &refM)
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

void DeviceRenderer::Render(const SceneNode &refNode, const Matrix4x4 &refM)
{
	Matrix4x4 transformation;

	transformation = refNode.GetTransformation() * refM;

	//enable lights
	for(Light *const& refpLight : refNode.GetAttachedLights())
	{
		this->EnableLight(refpLight);
	}

	//render entities
	for(Entity *const& refpEntity : refNode.GetAttachedEntities())
	{
		this->Render(*refpEntity, transformation);
	}

	//render children
	for(SceneNode *const& refpChild : refNode.GetChildren())
	{
		this->Render(*refpChild, transformation);
	}

	//disable lights
	for(Light *const& refpLight : refNode.GetAttachedLights())
	{
		this->EnableLight(refpLight, false);
	}
}

void DeviceRenderer::RenderMesh(const Mesh *pMesh)
{
	if(!this->meshObjects.Contains(pMesh))
		this->SetupMeshObjects(pMesh, this->meshObjects[pMesh]);

	SMeshObjects &refMeshObj = this->meshObjects[pMesh];

	this->refDeviceContext.SetInputState(refMeshObj.pInputState);

	this->refDeviceContext.DrawTrianglesIndexed();
}

void DeviceRenderer::RenderShadowMap(const Light *pLight, const SceneNode &refNode, const Matrix4x4 &refM)
{
	Camera lightCam;

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
	refLightInfo.lightVP = Matrix4x4::OrthographicRH(-10, 10, -10, 10, 0.1f, 1000.0f) * lightCam.GetViewMatrix();

	this->shadowPass.RenderDepthMap(refLightInfo, refNode, refM);
}

void DeviceRenderer::SetMaterial(const Material *pMaterial)
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

void DeviceRenderer::SetSkyBox(const SkyBox &refSkyBox)
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

void DeviceRenderer::SetTexture(const Texture *pTexture)
{
	if(!pTexture)
	{
		//this->refDeviceContext.SetTexture(TEXTURE_UNIT_MATERIAL, nullptr);
		return;
	}

	if(!this->textures.Contains(pTexture))
	{
		Texture2D *pDeviceTexture;

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

void DeviceRenderer::SetupMeshObjects(const Mesh *pMesh, SMeshObjects &refMeshObjects)
{
	//vertex buffer
	refMeshObjects.pVertexBuffer = this->refDeviceContext.CreateVertexBuffer();

	refMeshObjects.pVertexBuffer->Allocate(pMesh->GetNumberOfVertices(), sizeof(Mesh::SVertex), pMesh->GetVertices());

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

void DeviceRenderer::SetupFrameBuffer()
{
	Size screenSize;
	InputLayout inputLayout;

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
	screenSize = Size::GetScreenSize();

	this->pFrameColorBuffer = this->refDeviceContext.CreateTexture2D();
	this->pFrameColorBuffer->AllocateRGB(screenSize, nullptr);
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

void DeviceRenderer::SetupSkyBox()
{
	InputLayout inputLayout;

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