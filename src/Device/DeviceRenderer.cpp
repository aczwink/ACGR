/*
 * Copyright (c) 2017-2019 Amir Czwink (amir130@hotmail.de)
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
//Shaders
#include "../../shaders/Frame.h"
#include "../../shaders/Normals/FS.h"
#include "../../shaders/Normals/GS.h"
#include "../../shaders/Normals/VS.hpp"
#include "../../shaders/SkyBox.h"
//Namespaces
using namespace ACGR;

//Constructor
DeviceRenderer::DeviceRenderer(DeviceContext &refDeviceContext) : refDeviceContext(refDeviceContext), shaderCompiler(refDeviceContext), shadowPass(*this)
{
	this->debugMode = false;

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
void DeviceRenderer::EnableDebugMode(bool state)
{
	this->debugMode = state;
}

void DeviceRenderer::InformDeviceStateChanged(const SizeS &size)
{
	this->deviceSize = size;

	//resize all shadow depth maps
	for(auto &refKV : this->additionalLightInfo)
		refKV.value.pShadowMap->AllocateDepth(size.width, size.height);

	//this->pFrameColorBuffer->Allocate(false, size.width, size.height, nullptr);
}

void DeviceRenderer::RenderFrame(const SceneManager &sceneManager, const Camera &camera)
{
	//we need this throughout the whole frame
	this->current.sceneManager = &sceneManager;
	this->current.camera = &camera;

	//set device settings
	this->refDeviceContext.EnableDepthTest();
	this->refDeviceContext.EnableFaceCulling();

	//first do shadow passes for all lights
	auto lights = sceneManager.GetLights();
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
	this->BeginRendering(sceneManager, camera);

	this->refDeviceContext.SetFrameBuffer(nullptr); //switch to screen frame buffer

	this->refDeviceContext.ClearColorBuffer(Color(0.1f, 0.1f, 0.1f, 1));
	this->refDeviceContext.ClearDepthBuffer();

	this->Render(*sceneManager.GetRootNode(), Matrix4S::Identity());

	this->EndRendering();
}

//Private methods
void DeviceRenderer::BeginRendering(const SceneManager &sceneManager, const Camera &camera)
{
	//camera
	this->view = camera.GetViewMatrix();
	this->projection = camera.GetPerspectiveMatrix();
	this->VP = this->projection * this->view;

	//set the sky box
	this->SetSkyBox(sceneManager.GetSkyBox());
	this->refDeviceContext.SetTexture(TEXTURE_UNIT_ENVIRONMENT, this->skyBox.pCubeMap);
}

void DeviceRenderer::EnableLight(const Light *light, bool state)
{
	if(state)
		this->activeLights.Insert(light);
	else
		this->activeLights.Remove(light);
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
	this->pFrameProgram = this->shaderCompiler.CompileStaticProgram(SHADER_ARGS(frame));
	this->debug.normalsProgram = this->shaderCompiler.CompileStaticProgram(SHADER_ARGS_WITH_GEOMETRY(normals));

	this->meshInputLayout.AddAttribute3(); //pos
	this->meshInputLayout.AddAttribute3(); //normal
	this->meshInputLayout.AddAttribute2(); //texcoords
}

void DeviceRenderer::Render(const Mesh *pMesh, const Matrix4S &model)
{
	//program
	ShaderProgram *meshProgram = this->shaderCompiler.GetMeshProgram(this->current.material, this->activeLights);

	meshProgram->SetUniformValue(meshProgram->GetUniformId(u8"model"), model);
	meshProgram->SetUniformValue(meshProgram->GetUniformId(u8"MVP"), this->VP * model);
	meshProgram->SetUniformValue(meshProgram->GetUniformId("material.diffuseColor"), this->current.material->diffuseColor);

	meshProgram->SetUniformValue(meshProgram->GetUniformId(u8"cameraPos"), this->current.camera->position);
	meshProgram->SetUniformValue(meshProgram->GetUniformId(u8"ambientLight"), this->current.sceneManager->ambientLight);

	uint32 lightCounter = 0;
	for(const Light *const& light : this->activeLights)
	{
		ByteString prefix;

		SLightInfo &refLightInfo = this->additionalLightInfo[light];

		//set the light in shader
		prefix = "lights[" + ToString_8Bit(lightCounter) + "].";
		meshProgram->SetUniformValue(meshProgram->GetUniformId(prefix + "type"), (uint32)light->type);
		meshProgram->SetUniformValue(meshProgram->GetUniformId(prefix + "diffuseColor"), light->color);
		meshProgram->SetUniformValue(meshProgram->GetUniformId(prefix + "power"), light->power);
		meshProgram->SetUniformValue(meshProgram->GetUniformId(prefix + "lightVP"), refLightInfo.lightVP);
		meshProgram->SetUniformValue(meshProgram->GetUniformId(prefix + "direction"), light->direction);
		meshProgram->SetUniformValue(meshProgram->GetUniformId(prefix + "pos"), light->position);
		meshProgram->SetUniformValue(meshProgram->GetUniformId(prefix + "innerConeAngle"), (float32)cos(Radian(light->innerConeAngle).value));
		meshProgram->SetUniformValue(meshProgram->GetUniformId(prefix + "openingAngle"), (float32)cos(Radian(light->openingAngle).value));

		//set shadow map
		this->refDeviceContext.SetTexture(uint8(TEXTURE_UNIT_SHADOWMAP_0 + lightCounter), refLightInfo.pShadowMap);
		meshProgram->SetUniformValue(meshProgram->GetUniformId(prefix + "shadowMap"), int32(TEXTURE_UNIT_SHADOWMAP_0 + lightCounter));

		lightCounter++;
	}

	this->refDeviceContext.SetProgram(meshProgram);

	//render mesh
	this->RenderMesh(pMesh);

	if(this->debugMode)
	{
		this->debug.normalsProgram->SetUniformValue(this->debug.normalsProgram->GetUniformId(u8"model"), model);
		this->debug.normalsProgram->SetUniformValue(this->debug.normalsProgram->GetUniformId(u8"MVP"), this->VP * model);
		this->debug.normalsProgram->SetUniformValue(this->debug.normalsProgram->GetUniformId("view"), this->view);
		this->debug.normalsProgram->SetUniformValue(this->debug.normalsProgram->GetUniformId("projection"), this->projection);

		this->refDeviceContext.SetProgram(this->debug.normalsProgram);
		this->refDeviceContext.DrawTrianglesIndexed();
	}
}

void DeviceRenderer::Render(const SceneNode &refNode, const Matrix4S &refM)
{
	Matrix4S transformation;

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

void DeviceRenderer::RenderShadowMap(const Light *pLight, const SceneNode &refNode, const Matrix4S &refM)
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
	refLightInfo.lightVP = Matrix4S::OrthographicRH(-10, 10, -10, 10, 0.1f, 1000.0f) * lightCam.GetViewMatrix();

	this->shadowPass.RenderDepthMap(refLightInfo, refNode, refM);
}

void DeviceRenderer::SetMaterial(const Material *material)
{
	if(!material)
	{
		//no material available
		static const Material emptyMaterial;

		material = &emptyMaterial;
	}

	this->current.material = material;

	this->refDeviceContext.EnableBlending(material->useAlphaBlending);
	this->SetTexture(material->GetTexture());
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

	refMeshObjects.pVertexBuffer->Allocate(pMesh->GetNumberOfVertices(), sizeof(Mesh::Vertex), pMesh->GetVertices());

	//index buffer
	refMeshObjects.pIndexBuffer = this->refDeviceContext.CreateIndexBuffer();

	if(pMesh->GetNumberOfVertices() > UINT16_MAX) //refMeshObjects.pIndexBuffer->Allocate(pMesh->GetNumberOfIndices(), pMesh->GetIndices32());
		NOT_IMPLEMENTED_ERROR;
	else
		refMeshObjects.pIndexBuffer->Allocate16(pMesh->GetNumberOfIndices(), pMesh->GetIndices16());

	//input state
	refMeshObjects.pInputState = this->refDeviceContext.CreateInputState();
	refMeshObjects.pInputState->AddVertexBuffer(refMeshObjects.pVertexBuffer, this->meshInputLayout);
	refMeshObjects.pInputState->SetIndexBuffer(refMeshObjects.pIndexBuffer);
}

void DeviceRenderer::SetupFrameBuffer()
{
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

	//set up diffuseColor buffer for frame buffer
	this->pFrameColorBuffer = this->refDeviceContext.CreateTexture2D();
	//this->pFrameColorBuffer->AllocateRGB(screenSize.Cast<uint16>(), nullptr);
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
	this->skyBox.pProgram = this->shaderCompiler.CompileStaticProgram(SHADER_ARGS(skybox));

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