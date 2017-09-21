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
#include "ShadowPass.hpp"
//Local
#include "../DeviceRenderer.hpp"
//Shaders
#include "../../../shaders/Shadow.h"
//Namespaces
using namespace ACGR;

//Constructor
ShadowPass::ShadowPass(DeviceRenderer &refRenderer) : refRenderer(refRenderer)
{
	this->pShadowProgram = this->refRenderer.GetShaderCompiler().CompileStaticProgram(SHADER_ARGS(shadow));

	this->pFrameBuffer = this->refRenderer.GetDeviceContext().CreateFrameBuffer();

	this->pFrameBuffer->SetColorBuffer(nullptr);
}

//Destructor
ShadowPass::~ShadowPass()
{
	delete this->pFrameBuffer;
}

//Public methods
void ShadowPass::RenderDepthMap(SLightInfo &refLightInfo, const SceneNode &refNode, const Matrix4x4 &refM)
{
	DeviceContext &refDC = this->refRenderer.GetDeviceContext();

	refDC.SetProgram(this->pShadowProgram);
	this->pShadowProgram->SetUniformValue(0, refLightInfo.lightVP);

	refDC.SetFrameBuffer(this->pFrameBuffer);
	this->pFrameBuffer->SetDepthBuffer(refLightInfo.pShadowMap);

	refDC.ClearDepthBuffer();

	this->RenderNode(refNode, refM);
}

//Private methods
void ShadowPass::RenderNode(const SceneNode &refNode, const Matrix4x4 &refM)
{
	Matrix4x4 transformation;

	transformation = refNode.GetTransformation() * refM;

	//render entities
	for(Entity *const& refpEntity : refNode.GetAttachedEntities())
	{
		this->pShadowProgram->SetUniformValue(1, transformation);

		this->refRenderer.RenderMesh(refpEntity->GetMesh());
	}

	//render children
	for(SceneNode *const& refpChild : refNode.GetChildren())
	{
		this->RenderNode(*refpChild, transformation);
	}
}