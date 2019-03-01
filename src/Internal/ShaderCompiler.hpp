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
#include <Std++.hpp>
#include <ACGR/Light.hpp>
#include <ACGR/Material.hpp>
using namespace StdXX;
using namespace StdXX::Rendering;
using namespace ACGR;

#define SHADER_ARGS(inname) shader_ ## inname ## _vs, sizeof(shader_ ## inname ## _vs), shader_ ## inname ## _fs, sizeof(shader_ ## inname ## _fs)
#define SHADER_ARGS_WITH_GEOMETRY(inname) SHADER_ARGS(inname), shader_ ## inname ## _gs, sizeof(shader_ ## inname ## _gs)

enum TextureUnitAllocation_Mesh
{
	TEXTURE_UNIT_MATERIAL,
	TEXTURE_UNIT_ENVIRONMENT,

	//let the shadow maps be the last ones
	TEXTURE_UNIT_SHADOWMAP_0,
};

class ShaderCompiler
{
	class MeshParams
	{
	public:
		const Material *material;
		const BinaryTreeSet<const Light *> &activeLights;

		inline MeshParams(const Material *material, const BinaryTreeSet<const Light *> &activeLights) : material(material), activeLights(activeLights)
		{
		}

		inline bool operator<(const MeshParams &meshParams) const
		{
			return (this->activeLights.GetNumberOfElements() < meshParams.activeLights.GetNumberOfElements()) && (this->HasTexture() < meshParams.HasTexture());
		}

		inline bool operator>(const MeshParams &meshParams) const
		{
			return (this->activeLights.GetNumberOfElements() > meshParams.activeLights.GetNumberOfElements()) && (this->HasTexture() > meshParams.HasTexture());
		}

	private:
		inline bool HasTexture() const
		{
			return this->material->GetTexture() != nullptr;
		}
	};
public:
	//Constructor
	inline ShaderCompiler(DeviceContext &deviceContext) : deviceContext(deviceContext)
	{
	}

	//Destructor
	~ShaderCompiler();

	//Methods
	ShaderProgram *CompileStaticProgram(const char *vsShaderCode, uint32 vsShaderSize, const char *fsShaderCode, uint32 fsShaderSize, const char *gsShaderCode = nullptr, uint32 gsShaderSize = 0);
	ShaderProgram *GetMeshProgram(const Material *material, const BinaryTreeSet<const Light *> &activeLights);

private:
	//Members
	DeviceContext &deviceContext;
	DynamicArray<ShaderProgram *> staticPrograms;
	Map<MeshParams, ShaderProgram *> meshPrograms;

	//Methods
	ShaderProgram *CompileMeshProgram(const MeshParams &meshParams);
	ShaderProgram *CompileProgram(const char *vsShaderCode, uint32 vsShaderSize, const char *fsShaderCode, uint32 fsShaderSize, const char *gsShaderCode, uint32 gsShaderSize);
};