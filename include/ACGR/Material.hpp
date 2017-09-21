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
#include "Texture.hpp"

namespace ACGR
{
	class Material
	{
	public:
		//Members
		ACStdLib::Math::Vector3 diffuseColor;
		bool useAlphaBlending;

		//Constructor
		inline Material()
		{
			this->useAlphaBlending = false;
			this->texture = nullptr;
		}

		//Inline
		inline const Texture *GetTexture() const
		{
			return this->texture;
		}

	private:
		//Members
		Texture *texture;
	};
}