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
	//Forward declarations
	class Material;

	class Mesh
	{
	public:
		struct SVertex
		{
			ACStdLib::Math::Vector3 position;
			ACStdLib::Math::Vector3 normal;
			ACStdLib::Math::Vector2 textureCoords;
		};

		//Inline
		inline const Material *GetMaterial() const
		{
			return this->pMaterial;
		}

		inline uint16 *GetIndices16() const
		{
			return this->pIndices16;
		}

		inline uint32 *GetIndices32() const
		{
			return this->pIndices32;
		}

		inline uint32 GetNumberOfIndices() const
		{
			return this->nIndices;
		}

		inline uint32 GetNumberOfVertices() const
		{
			return this->nVertices;
		}

		inline const SVertex *GetVertices() const
		{
			return this->pVertices;
		}

	private:
		//Members
		uint32 nVertices;
		SVertex *pVertices;
		uint32 nIndices;
		union
		{
			uint16 *pIndices16;
			uint32 *pIndices32;
		};
		Material *pMaterial;
	};
}