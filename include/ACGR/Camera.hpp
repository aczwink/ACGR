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
		ACStdLib::Math::Matrix4x4 GetViewMatrix() const;
		void SetViewDirection(const ACStdLib::Math::Vector3 &refDir);

		//Inline
		inline void LookAt(const ACStdLib::Math::Vector3 &refTarget)
		{
			this->SetViewDirection(refTarget - this->position);
		}

		inline void SetFieldOfView(const ACStdLib::Degree &refFieldOfView)
		{
			this->fieldOfViewY = refFieldOfView;
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
		ACStdLib::Math::Matrix4x4 GetOrientation() const;
		void NormalizeAngles();
	};
}