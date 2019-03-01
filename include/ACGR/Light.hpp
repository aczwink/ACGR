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

namespace ACGR
{
	enum class ELightType
	{
		Directional,
		Point,
		Spot
	};

	class Light
	{
	public:
		//Members
		ELightType type;
		StdXX::Math::Vector3S direction; //For directional and Spotlight
		StdXX::Math::Vector3S position; //For Point and Spotlight
		StdXX::Math::Vector3S color;
		float32 power;
		StdXX::Math::Degree<float32> openingAngle; //For Spotlight
		StdXX::Math::Degree<float32> innerConeAngle; //For Spotlight

		//Constructor
		inline Light()
		{
			this->type = ELightType::Point;
			this->power = 1;
		}

		//Inline
		inline void LookAt(const StdXX::Math::Vector3S &refTarget)
		{
			this->direction = refTarget - this->position;
		}
	};
}