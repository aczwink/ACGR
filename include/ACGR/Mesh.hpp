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
#include <ACStdLib.hpp>

namespace ACGR
{
	//Forward declarations
	class Material;

	class Mesh
	{
	public:
		struct Vertex
		{
			ACStdLib::Math::Vector3 position;
			ACStdLib::Math::Vector3 normal;
			ACStdLib::Math::Vector2 textureCoords;
		};

		//Constructor
		Mesh();

		//Destructor
		~Mesh();

		//Methods
		void SetIndices(uint16 nIndices, const uint16 *pIndices);
		void SetVertices(uint32 nVertices, const Vertex *pVertices);

		//Inline
		inline const ACStdLib::Math::AxisAlignedBox &GetBoundingBox() const
		{
			return this->bbox;
		}

		inline const Material *GetMaterial() const
		{
			return this->pMaterial;
		}

		inline uint16 *GetIndices16() const
		{
			return this->pIndices16;
		}

		/*
		inline uint32 *GetIndices32() const
		{
			return this->pIndices32;
		}
		*/

		inline uint32 GetNumberOfIndices() const
		{
			return this->nIndices;
		}

		inline uint32 GetNumberOfVertices() const
		{
			return this->nVertices;
		}

		inline Vertex *GetVertices()
		{
			return this->pVertices;
		}

		inline const Vertex *GetVertices() const
		{
			return this->pVertices;
		}

	protected:
		//Members
		ACStdLib::Math::AxisAlignedBox bbox;

		//Methods
		void ComputeBBox();
		void ComputeNormals();
		void Subdivide();

	private:
		//Members
		uint32 nVertices;
		Vertex *pVertices;
		uint32 nIndices;
		union
		{
			uint16 *pIndices16;
			//uint32 *pIndices32;
		};
		Material *pMaterial;
	};
}