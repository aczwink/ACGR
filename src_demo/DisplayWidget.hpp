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
#include <ACGR.hpp>
using namespace ACStdLib;
using namespace ACStdLib::Math;
using namespace ACStdLib::UI;
using namespace ACStdLib::Rendering;
using namespace ACGR;

class DisplayWidget : public RenderTargetWidget
{
public:
    //Constructor
    DisplayWidget(WidgetContainer *parent);

    //Destructor
    ~DisplayWidget();

	//Inline
	inline void RayTrace()
	{
		this->rayTracer->RenderFrame(this->sceneMgr, *this->camera);
		this->deviceContext->SwapBuffers();

		this->haveRenderedImage = true;
	}

	inline void SetDebugMode(bool enabled)
	{
		this->renderer->EnableDebugMode(enabled);
	}

	inline void SetWireFrame(bool enabled)
	{
		this->deviceContext->EnablePolygonFilling(!enabled);
		this->Repaint();
	}

private:
	//Members
	bool haveRenderedImage; //other than by default
	Renderer *renderer;
	Renderer *rayTracer;
	SceneManager sceneMgr;
	Clock clock;
	FreeFlyCamera *camera;
	Map<String, Material *> materials;
	Map<String, Mesh *> meshes;

	//Eventhandlers
	void OnPaint();
	void OnResized();

	//Methods
	void InitScene();
	float64 ParseAngle(const String &string);
	void ParseRotation(const XML::Element &element, SceneNode *node);
	Vector3 ParseVec3(const String &refString);
	void ReadScene(InputStream &inputStream);
	SceneNode *ReadSceneNode(const XML::Element &element);
	void UpdateScene();
};