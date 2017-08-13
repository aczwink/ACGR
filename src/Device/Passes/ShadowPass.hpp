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
#pragma once
#include <ACStdLib.hpp>
//Local
#include <ACGR/SceneNode.hpp>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Math;
using namespace ACStdLib::Rendering;

namespace ACGR
{
	//Forward declarations
	class DeviceRenderer;
	struct SLightInfo;

	class ShadowPass
	{
	public:
		//Constructor
		ShadowPass(DeviceRenderer &refRenderer);

		//Destructor
		~ShadowPass();

		//Methods
		void RenderDepthMap(SLightInfo &refLightInfo, const SceneNode &refNode, const Matrix4x4 &refM);

	private:
		//Members
		ShaderProgram *pShadowProgram;
		IFrameBuffer *pFrameBuffer;

		DeviceRenderer &refRenderer;

		//Methods
		void RenderNode(const SceneNode &refNode, const Matrix4x4 &refM);
	};
}