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
#include "DisplayWidget.hpp"
//Namespaces
using namespace StdXX::XML;

//Prototypes
void ReadScene(InputStream &inputStream, SceneManager &sceneManager);

//Constructor
DisplayWidget::DisplayWidget() : RenderTargetWidget()
{
	this->haveRenderedImage = false;
}

//Destructor
DisplayWidget::~DisplayWidget()
{
	for(auto &kv : this->materials)
		delete kv.value;

	for(auto &kv : this->meshes)
		delete kv.value;

	delete this->renderer;
	delete this->rayTracer;
}

//Private methods
void DisplayWidget::InitScene()
{
	//Load materials
	Material *tmp = new Material;
	tmp->diffuseColor = Vector3(0.75f, 0.75f, 0.75f);
	this->materials["cornell_box_grey"] = tmp;

	tmp = new Material;
	tmp->diffuseColor = Vector3(0.75f, 0.25f, 0.25f);
	this->materials["cornell_box_red"] = tmp;

	tmp = new Material;
	tmp->diffuseColor = Vector3(0.25f, 0.25f, 0.75f);
	this->materials["cornell_box_blue"] = tmp;

	tmp = new Material;
	tmp->diffuseColor = Vector3(0.25f, 0.75f, 0.25f);
	this->materials["cornell_box_green"] = tmp;

	//Load meshes
	this->meshes["Icosahedron"] = new Icosahedron();
	this->meshes["Plane"] = new Plane();
	this->meshes["Sphere"] = new IcoSphere(2);
	this->meshes["Triangle"] = new Triangle();

	//Load scene
	FileInputStream sceneDescription(Path("demo_scenes/cornell_box.xml"));
	this->ReadScene(sceneDescription);

	//find camera
	this->camera = (FreeFlyCamera *)this->sceneMgr.GetRootNode()->GetAttachedCameras()[0];

	//go
	this->clock.Start();
}

float64 DisplayWidget::ParseAngle(const String &string)
{
	if(string.IsEmpty())
		return 0;

	const uint8 degree[] = {0xC2, 0xB0, 0}; //degree sign
	if(string.EndsWith((const char*)degree))
	{
		String tmp;

		tmp = string.SubString(0, string.GetLength() - 1);

		return tmp.ToFloat();
	}

	return Degree(Radian(string.ToFloat())).value;
}

void DisplayWidget::ParseRotation(const Element &element, SceneNode *node)
{
	Vector3S v;

	if(element.HasAttribute("rotate_ypr"))
	{
		v = ParseVec3(element.GetAttribute("rotate_ypr"));
		node->Rotate(Degree(v.x), Degree(v.y), Degree(v.z));
	}
}

Vector3S DisplayWidget::ParseVec3(const String& string)
{
	Vector3S result;

	DynamicArray<String> parts = string.Split(u8",");
	for(uint8 i = 0; i < 3; i++)
	{
		result.e[i] = (float32)parts[i].ToFloat();
	}

	return result;
}

void DisplayWidget::ReadScene(InputStream &inputStream)
{
	XML::Document *doc = XML::Document::Parse(inputStream);

	XML::Element &scene = doc->GetRootElement();
	if(scene.HasAttribute("ambientLight"))
		this->sceneMgr.ambientLight = this->ParseVec3(scene.GetAttribute("ambientLight"));

	if(scene.HasAttribute("skybox"))
		this->sceneMgr.SetSkyBox(scene.GetAttribute("skybox"));

	this->sceneMgr.SetRootNode(this->ReadSceneNode(scene));

	delete doc;
}

SceneNode *DisplayWidget::ReadSceneNode(const XML::Element &element)
{
	SceneNode *node = new SceneNode;

	for(const Node *const& child : element)
	{
		if(child->GetType() == NodeType::Element)
		{
			const Element &childElement = (const Element &)*child;

			if(childElement.GetName() == "Camera")
			{
				Camera *pCamera;

				pCamera = new FreeFlyCamera; //TODO: cam type

				if(childElement.HasAttribute("fovy"))
					pCamera->SetFieldOfView(ParseAngle(childElement.GetAttribute("fovy")));
				if(childElement.HasAttribute("position"))
					pCamera->position = ParseVec3(childElement.GetAttribute("position"));
				if(childElement.HasAttribute("direction"))
					pCamera->SetViewDirection(ParseVec3(childElement.GetAttribute("direction")));
				else if(childElement.HasAttribute("lookat"))
					pCamera->LookAt(ParseVec3(childElement.GetAttribute("lookat")));

				node->AttachCamera(pCamera);
			}
			else if(childElement.GetName() == "Entity")
			{
				TextNode *textNode = (TextNode *)childElement.GetChildren().Get(0);
				Mesh *mesh = this->meshes[textNode->GetText()];
				Entity *entity = new Entity(mesh);

				if(childElement.HasAttribute("material"))
					entity->SetMaterial(this->materials[childElement.GetAttribute("material")]);

				node->AttachEntity(entity);
			}
			else if(childElement.GetName() == "Light")
			{
				Light *pLight;

				pLight = new Light;

				if(childElement.GetAttribute("type") == "directional")
					pLight->type = ELightType::Directional;
				else if(childElement.GetAttribute("type") == "spot")
					pLight->type = ELightType::Spot;
				//else point

				if(childElement.HasAttribute("position"))
					pLight->position = ParseVec3(childElement.GetAttribute("position"));

				if(childElement.HasAttribute("direction"))
					pLight->direction = ParseVec3(childElement.GetAttribute("direction"));

				pLight->color = ParseVec3(childElement.GetAttribute("diffuseColor"));

				pLight->power = (float32)childElement.GetAttribute("power").ToFloat();

				if(childElement.HasAttribute("opening_angle"))
					pLight->openingAngle = ParseAngle(childElement.GetAttribute("opening_angle"));
				if(childElement.HasAttribute("inner_cone_angle"))
					pLight->innerConeAngle = ParseAngle(childElement.GetAttribute("inner_cone_angle"));

				//advanced values
				if(childElement.HasAttribute("lookat"))
				{
					Vector3S target = ParseVec3(childElement.GetAttribute("lookat"));

					pLight->LookAt(target);
				}

				node->AttachLight(pLight);
			}
			else if(childElement.GetName() == "Node")
			{
				SceneNode *child = ReadSceneNode(childElement);

				//first scale
				if(childElement.HasAttribute("scale"))
				{
					Vector3S v = ParseVec3(childElement.GetAttribute("scale"));
					child->Scale(v.x, v.y, v.z);
				}

				//then rotate
				ParseRotation(childElement, child);

				//then translate
				if(childElement.HasAttribute("translate"))
				{
					Vector3S v = ParseVec3(childElement.GetAttribute("translate"));
					child->Translate(v.x, v.y, v.z);
				}

				node->AddChild(child);
			}
		}
	}

	return node;
}

void DisplayWidget::UpdateScene()
{
	float64 dt = this->clock.GetElapsedNanoseconds() / 1000000000.0;
	this->clock.Start();

	//update camera
	//this->camera->Update(dt);
	//GetMouseButtonState(MouseButton::Left)
	// if(GetKeyState(KeyCode::Shift_Left) || GetKeyState(KeyCode::Shift_Right)) //if(GetKeyState(KeyCode::W))
}

//Eventhandlers
void DisplayWidget::OnPaint(UI::Event& event)
{
	this->UpdateScene();

	if(!this->haveRenderedImage)
	{
		this->renderer->RenderFrame(this->sceneMgr, *this->camera);
		this->deviceContext->SwapBuffers();
	}
	event.Accept();
}

void DisplayWidget::OnRealized()
{
	Widget::OnRealized();

	this->renderer = Renderer::CreateInstance(RendererType::Device, *this->deviceContext);
	this->rayTracer = Renderer::CreateInstance(RendererType::RayTracer, *this->deviceContext);

	this->InitScene();
}

void DisplayWidget::OnResized()
{
	RenderTargetWidget::OnResized();

	this->haveRenderedImage = false;

	this->renderer->InformDeviceStateChanged(this->GetSize().Cast<float32>());
	this->rayTracer->InformDeviceStateChanged(this->GetSize().Cast<float32>());

	this->camera->SetAspectRatio(this->GetSize().width / (float32)this->GetSize().height);

	this->Repaint();
}