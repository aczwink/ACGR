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
#include <ACGR/Renderer.hpp>
//Local
#include "Device/DeviceRenderer.hpp"
//Namespaces
using namespace ACGR;

//Global functions
Renderer *Renderer::CreateInstance(RendererType type, ACStdLib::Rendering::DeviceContext &deviceContext)
{
	switch(type)
	{
		case RendererType::Device:
			return new DeviceRenderer(deviceContext);
		case RendererType::RayTracer:
			NOT_IMPLEMENTED_ERROR;
			//return new CRayTraceRenderer(deviceContext);
	}

	return nullptr;
}