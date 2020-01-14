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
#include <ACGR.hpp>
using namespace StdXX;
using namespace StdXX::Math;
using namespace StdXX::UI;
using namespace StdXX::Rendering;
using namespace ACGR;

class DisplayWidget : public RenderTargetWidget
{
public:
    //Constructor
    DisplayWidget();

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
	void OnPaint(PaintEvent& event) override;
	void OnRealized() override;
	void OnResized() override;

	//Methods
	void InitScene();
	float64 ParseAngle(const String &string);
	void ParseRotation(const XML::Element &element, SceneNode *node);
	Vector3S ParseVec3(const String &string);
	void ReadScene(InputStream &inputStream);
	SceneNode *ReadSceneNode(const XML::Element &element);
	void UpdateScene();
};