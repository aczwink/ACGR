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
#include <ACGR/SceneNode.hpp>
//Local
#include <ACGR/Cameras/FreeFlyCamera.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Math;
using namespace StdXX::XML;
using namespace ACGR;

//Constructor
SceneNode::SceneNode()
{
	this->pParent = nullptr;
	this->transformation = Matrix4S::Identity();
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
Matrix4S SceneNode::ComputeModelMatrix() const
{
	SceneNode *pNode;
	Matrix4S model;

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