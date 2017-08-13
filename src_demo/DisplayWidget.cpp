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
#include "DisplayWidget.hpp"
//Namespaces
using namespace ACStdLib::Math;

//Constructor
DisplayWidget::DisplayWidget(WidgetContainer *parent) : RenderTargetWidget(parent)
{
	this->haveRenderedImage = false;

	this->renderer = Renderer::CreateInstance(RendererType::Device, *this->deviceContext);

	this->InitScene();
}

//Destructor
DisplayWidget::~DisplayWidget()
{
	delete this->renderer;
}

//Eventhandlers
void DisplayWidget::OnPaint()
{
	this->UpdateScene();

	if(!this->haveRenderedImage)
	{
		this->renderer->RenderFrame(this->sceneMgr, *this->camera);
		this->deviceContext->SwapBuffers();
	}
}

//Private methods
void DisplayWidget::InitScene()
{
	//Load scene
	FileInputStream sceneDescription(Path("scenes/cornell_box.xml"));
	this->sceneMgr.Load(sceneDescription);

	//find camera
	this->camera = (FreeFlyCamera *)this->sceneMgr.GetRootNode()->GetAttachedCameras()[0];
}

void DisplayWidget::UpdateScene()
{
}