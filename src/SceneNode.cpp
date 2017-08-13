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
#include <ACGR/SceneNode.hpp>
//Local
#include <ACGR/FreeFlyCamera.hpp>
#include "Internal/Parse.hpp"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Math;
using namespace ACStdLib::XML;
using namespace ACGR;

//Constructor
SceneNode::SceneNode()
{
	this->pParent = nullptr;
	this->transformation = Matrix4x4::Identity();
}

//Destructor
SceneNode::~SceneNode()
{
	for(const Camera *const& refpCamera : this->attachedCameras)
	{
		delete refpCamera;
	}

	for(const Entity *const& refpEntity : this->attachedEntities)
	{
		delete refpEntity;
	}

	for(const Light *const& refpLight : this->attachedLights)
	{
		delete refpLight;
	}

	for(const SceneNode *const& refpChild : this->children)
	{
		delete refpChild;
	}
}

//Public methods
Matrix4x4 SceneNode::ComputeModelMatrix() const
{
	SceneNode *pNode;
	Matrix4x4 model;

	model = this->transformation;
	pNode = this->pParent;
	while(pNode)
	{
		model = pNode->transformation * model;
		pNode = pNode->pParent;
	}

	return model;
}

//Private methods
void SceneNode::Deserialize(const CElement &refElement)
{
	UTF8String text;

	for(const ANode *const& refpChild : refElement)
	{
		if(refpChild->GetType() == ENodeType::NODE_TYPE_ELEMENT)
		{
			const CElement &refChildElement = (const CElement &)*refpChild;

			if(refChildElement.GetName() == "Camera")
			{
				Camera *pCamera;

				pCamera = new FreeFlyCamera; //TODO: cam type
				this->AttachCamera(pCamera);

				if(refChildElement.HasAttribute("fovy"))
					pCamera->SetFieldOfView(this->ParseAngle(refChildElement.GetAttribute("fovy")));
				if(refChildElement.HasAttribute("position"))
					pCamera->position = ParseVec3(refChildElement.GetAttribute("position"));
				if(refChildElement.HasAttribute("direction"))
					pCamera->SetViewDirection(ParseVec3(refChildElement.GetAttribute("direction")));
				else if(refChildElement.HasAttribute("lookat"))
					pCamera->LookAt(ParseVec3(refChildElement.GetAttribute("lookat")));
			}
			else if(refChildElement.GetName() == "Entity")
			{
				Entity *pEntity;
				CTextNode *pTextNode;

				pTextNode = (CTextNode *)refChildElement.GetChildren().Get(0);
				text = pTextNode->GetText();

				pEntity = new Entity(ByteString((char *)text.GetC_Str()));
				this->AttachEntity(pEntity);

				if(refChildElement.HasAttribute("material"))
					pEntity->SetMaterial(To8BitString(refChildElement.GetAttribute("material")));
			}
			else if(refChildElement.GetName() == "Light")
			{
				Light *pLight;

				pLight = new Light;

				if(refChildElement.GetAttribute("type") == "directional")
					pLight->type = ELightType::Directional;
				else if(refChildElement.GetAttribute("type") == "spot")
					pLight->type = ELightType::Spot;
				//else point

				if(refChildElement.HasAttribute("position"))
					pLight->position = ParseVec3(refChildElement.GetAttribute("position"));

				if(refChildElement.HasAttribute("direction"))
					pLight->direction = ParseVec3(refChildElement.GetAttribute("direction"));

				pLight->color = ParseVec3(refChildElement.GetAttribute("color"));

				pLight->power = (float32)StringToFloat64(refChildElement.GetAttribute("power"));

				if(refChildElement.HasAttribute("opening_angle"))
					pLight->openingAngle = this->ParseAngle(refChildElement.GetAttribute("opening_angle"));
				if(refChildElement.HasAttribute("inner_cone_angle"))
					pLight->innerConeAngle = this->ParseAngle(refChildElement.GetAttribute("inner_cone_angle"));

				//advanced values
				if(refChildElement.HasAttribute("lookat"))
				{
					Vector3 target;

					target = ParseVec3(refChildElement.GetAttribute("lookat"));

					pLight->LookAt(target);
				}

				this->AttachLight(pLight);
			}
			else if(refChildElement.GetName() == "Node")
			{
				SceneNode *pNode;
				Vector3 v;

				pNode = new SceneNode;
				pNode->Deserialize(refChildElement);

				//first scale
				if(refChildElement.HasAttribute("scale"))
				{
					v = ParseVec3(refChildElement.GetAttribute("scale"));
					pNode->Scale(v.x, v.y, v.z);
				}

				//then rotate
				pNode->ParseRotation(refChildElement);

				//then translate
				if(refChildElement.HasAttribute("translate"))
				{
					v = ParseVec3(refChildElement.GetAttribute("translate"));
					pNode->Translate(v.x, v.y, v.z);
				}

				this->AddChild(pNode);
			}
		}
	}
}

void SceneNode::GetLights(Map<Light *, SceneNode *> &refResult)
{
	for(Light *const& refpLight : this->attachedLights)
	{
		refResult.Insert(refpLight, this);
	}

	for(SceneNode *const& refpChild : this->children)
	{
		refpChild->GetLights(refResult);
	}
}