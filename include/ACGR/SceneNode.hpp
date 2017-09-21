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
//Local
#include "Entity.hpp"
#include "Light.hpp"

namespace ACGR
{
	//Forward declarations
	class Camera;

	class SceneNode
	{
		friend class SceneManager;
	public:
		//Methods
		ACStdLib::Math::Matrix4x4 ComputeModelMatrix() const;

		//Constructor
		SceneNode();

		//Destructor
		~SceneNode();

		//Inline
		inline void AddChild(SceneNode *pNode)
		{
			this->children.InsertTail(pNode);
			pNode->pParent = this;
		}

		inline void AttachCamera(Camera *pCamera)
		{
			this->attachedCameras.InsertTail(pCamera);
		}

		inline void AttachEntity(Entity *pEntity)
		{
			this->attachedEntities.InsertTail(pEntity);
		}

		inline void AttachLight(Light *pLight)
		{
			this->attachedLights.InsertTail(pLight);
		}

		inline const ACStdLib::LinkedList<Camera *> &GetAttachedCameras() const
		{
			return this->attachedCameras;
		}

		inline const ACStdLib::LinkedList<Entity *> &GetAttachedEntities() const
		{
			return this->attachedEntities;
		}

		inline const ACStdLib::LinkedList<Light *> &GetAttachedLights() const
		{
			return this->attachedLights;
		}

		inline const ACStdLib::LinkedList<SceneNode *> &GetChildren() const
		{
			return this->children;
		}

		inline ACStdLib::Map<Light *, SceneNode *> GetLights()
		{
			ACStdLib::Map<Light *, SceneNode *> result;
			this->GetLights(result);
			return result;
		}

		inline const ACStdLib::Math::Matrix4x4 &GetTransformation() const
		{
			return this->transformation;
		}

		inline void Rotate(const ACStdLib::Radian &refYaw, const ACStdLib::Radian &refPitch, const ACStdLib::Radian &refRoll)
		{
			this->transformation *= ACStdLib::Math::Matrix4x4::RotationYawPitchRoll(refYaw, refPitch, refRoll);
		}

		inline void Scale(float32 scaleX, float32 scaleY, float32 scaleZ)
		{
			this->transformation *= ACStdLib::Math::Matrix4x4::Scale(scaleX, scaleY, scaleZ);
		}

		inline void Translate(float32 dx, float32 dy, float32 dz)
		{
			this->transformation *= ACStdLib::Math::Matrix4x4::Translation(dx, dy, dz);
		}

	private:
		//Members
		SceneNode *pParent;
		ACStdLib::Math::Matrix4x4 transformation;
		ACStdLib::LinkedList<SceneNode *> children;
		ACStdLib::LinkedList<Camera *> attachedCameras;
		ACStdLib::LinkedList<Entity *> attachedEntities;
		ACStdLib::LinkedList<Light *> attachedLights;

		//Methods
		void GetLights(ACStdLib::Map<Light *, SceneNode *> &refResult);
	};
}