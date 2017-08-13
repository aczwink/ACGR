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
		ACStdLib::Math::Vector3 direction; //For directional and Spotlight
		ACStdLib::Math::Vector3 position; //For Point and Spotlight
		ACStdLib::Math::Vector3 color;
		float32 power;
		ACStdLib::Degree openingAngle; //For Spotlight
		ACStdLib::Degree innerConeAngle; //For Spotlight

		//Constructor
		inline Light()
		{
			this->type = ELightType::Point;
			this->power = 1;
		}

		//Inline
		inline void LookAt(const ACStdLib::Math::Vector3 &refTarget)
		{
			this->direction = refTarget - this->position;
		}
	};
}