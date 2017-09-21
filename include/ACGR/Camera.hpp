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
#include <ACStdLib.hpp>

namespace ACGR
{
	class Camera
	{
	public:
		//Constructor
		Camera();

		//Members
		ACStdLib::Math::Vector3 position;

		//Methods
		ACStdLib::Math::Matrix4x4 GetPerspectiveMatrix() const;
		void SetViewDirection(const ACStdLib::Math::Vector3 &refDir);

		//Inline
		inline ACStdLib::Math::Vector3 GetForwardDirection() const
		{
			return this->GetInverseOrientation() * ACStdLib::Math::Vector4(0, 0, 1, 1);
		}

		inline ACStdLib::Math::Matrix4x4 GetViewMatrix() const
		{
			return this->GetOrientation() * ACStdLib::Math::Matrix4x4::Translation(-this->position);
		}

		inline ACStdLib::Math::Vector3 GetRightDirection() const
		{
			return this->GetInverseOrientation() * ACStdLib::Math::Vector4(1, 0, 0, 1);
		}

		inline void LookAt(const ACStdLib::Math::Vector3 &refTarget)
		{
			this->SetViewDirection(refTarget - this->position);
		}

		inline void Rotate(const ACStdLib::Degree &refDx, const ACStdLib::Degree &refDy)
		{
			this->horzAngle += refDx;
			this->vertAngle += refDy;

			this->NormalizeAngles();
		}

		inline void SetAspectRatio(float32 aspectRatio)
		{
			this->aspectRatio = aspectRatio;
		}

		inline void SetFieldOfView(const ACStdLib::Degree &refFieldOfView)
		{
			this->fieldOfViewY = refFieldOfView;
		}

		inline void Translate(const ACStdLib::Math::Vector3 &refVector)
		{
			this->position += refVector;
		}

	private:
		//Members
		ACStdLib::Degree horzAngle; //Clockwise (straight = 0), range [0, 360]
		ACStdLib::Degree vertAngle; //Clockwise (straight = 0, positive values = look down), range [-85, 85]
		ACStdLib::Degree fieldOfViewY;
		float32 aspectRatio;
		float32 nearPlane;
		float32 farPlane;

		//Methods
		void NormalizeAngles();

		//Inline
		inline ACStdLib::Math::Matrix4x4 GetInverseOrientation() const
		{
			return ACStdLib::Math::Matrix4x4::RotationYawPitchRoll(-this->horzAngle, -this->vertAngle, 0);
		}

		inline ACStdLib::Math::Matrix4x4 GetOrientation() const
		{
			return ACStdLib::Math::Matrix4x4::RotationYawPitchRoll(this->horzAngle, this->vertAngle, 0);
		}
	};
}