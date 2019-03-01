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
#include <Std++.hpp>

namespace ACGR
{
	class Camera
	{
	public:
		//Constructor
		Camera();

		//Members
		StdXX::Math::Vector3S position;

		//Methods
		StdXX::Math::Matrix4S GetPerspectiveMatrix() const;
		void SetViewDirection(const StdXX::Math::Vector3S &refDir);

		//Inline
		inline StdXX::Math::Vector3S GetForwardDirection() const
		{
			return this->GetInverseOrientation() * StdXX::Math::Vector4S(0, 0, 1, 1);
		}

		inline StdXX::Math::Matrix4S GetViewMatrix() const
		{
			return this->GetOrientation() * StdXX::Math::Matrix4S::Translation(-this->position);
		}

		inline StdXX::Math::Vector3S GetRightDirection() const
		{
			return this->GetInverseOrientation() * StdXX::Math::Vector4S(1, 0, 0, 1);
		}

		inline void LookAt(const StdXX::Math::Vector3S &refTarget)
		{
			this->SetViewDirection(refTarget - this->position);
		}

		inline void Rotate(const StdXX::Math::Degree<float32> &refDx, const StdXX::Math::Degree<float32> &refDy)
		{
			this->horzAngle += refDx;
			this->vertAngle += refDy;

			this->NormalizeAngles();
		}

		inline void SetAspectRatio(float32 aspectRatio)
		{
			this->aspectRatio = aspectRatio;
		}

		inline void SetFieldOfView(const StdXX::Math::Degree<float32> &refFieldOfView)
		{
			this->fieldOfViewY = refFieldOfView;
		}

		inline void Translate(const StdXX::Math::Vector3S &refVector)
		{
			this->position += refVector;
		}

	private:
		//Members
		StdXX::Math::Degree<float32> horzAngle; //Clockwise (straight = 0), range [0, 360]
		StdXX::Math::Degree<float32> vertAngle; //Clockwise (straight = 0, positive values = look down), range [-85, 85]
		StdXX::Math::Degree<float32> fieldOfViewY;
		float32 aspectRatio;
		float32 nearPlane;
		float32 farPlane;

		//Methods
		void NormalizeAngles();

		//Inline
		inline StdXX::Math::Matrix4S GetInverseOrientation() const
		{
			return StdXX::Math::Matrix4S::RotationYawPitchRoll(-this->horzAngle, -this->vertAngle, 0);
		}

		inline StdXX::Math::Matrix4S GetOrientation() const
		{
			return StdXX::Math::Matrix4S::RotationYawPitchRoll(this->horzAngle, this->vertAngle, 0);
		}
	};
}