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
//Local
#include "Mesh.hpp"

namespace ACGR
{
	class Entity
	{
	public:
		//Constructor
		inline Entity(const Mesh *mesh)
		{
			this->mesh = mesh;
			this->material = nullptr;
		}

		//Inline
		inline const Material *GetMaterial() const
		{
			return this->material;
		}

		inline const Mesh *GetMesh() const
		{
			return this->mesh;
		}

		inline void SetMaterial(const Material *material)
		{
			this->material = material;
		}

	private:
		//Members
		const Mesh *mesh;
		const Material *material;
	};
}