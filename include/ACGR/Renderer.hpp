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
#pragma once
#include <Std++.hpp>

namespace ACGR
{
	//Forward declarations
	class Camera;
	class Light;
	class Mesh;
	class SceneManager;

	enum class RendererType
	{
		Device,
		RayTracer
	};

	class Renderer
	{
	public:
		//Destructor
		virtual ~Renderer() {}

		//Abstract
		virtual void EnableDebugMode(bool state) = 0;
		virtual void InformDeviceStateChanged(const StdXX::Math::SizeS &size) = 0;
		virtual void RenderFrame(const SceneManager &sceneManager, const Camera &camera) = 0;

		//Functions
		static Renderer *CreateInstance(RendererType type, StdXX::Rendering::DeviceContext &deviceContext);
	};
}