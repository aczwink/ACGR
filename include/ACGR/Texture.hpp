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

namespace ACGR
{
	enum ETextureFormat
	{
		TEXTURE_FORMAT_DXT1,
		TEXTURE_FORMAT_DXT5
	};

	class Texture
	{
	public:
		//Inline
		inline const void *GetData() const
		{
			return this->pBuffer;
		}

		inline ETextureFormat GetFormat() const
		{
			return this->textureFormat;
		}

		inline uint16 GetHeight() const
		{
			return this->height;
		}

		inline uint16 GetWidth() const
		{
			return this->width;
		}

	private:
		//Members
		uint16 width;
		uint16 height;
		void *pBuffer;
		ETextureFormat textureFormat;
	};
}