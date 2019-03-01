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
//Class header
#include "ShaderCompiler.hpp"
//Shaders
#include "../../shaders/Mesh/FS.h"
#include "../../shaders/Mesh/VS.hpp"

//Destructor
ShaderCompiler::~ShaderCompiler()
{
	for(const auto &kv : this->meshPrograms)
		delete kv.value;

	for(const ShaderProgram *const& program : this->staticPrograms)
		delete program;
}

//Public methods
ShaderProgram *ShaderCompiler::CompileStaticProgram(const char *vsShaderCode, uint32 vsShaderSize, const char *fsShaderCode, uint32 fsShaderSize, const char *gsShaderCode, uint32 gsShaderSize)
{
	ShaderProgram *program = this->CompileProgram(vsShaderCode, vsShaderSize, fsShaderCode, fsShaderSize, gsShaderCode, gsShaderSize);
	this->staticPrograms.Push(program);

	return program;
}

ShaderProgram *ShaderCompiler::GetMeshProgram(const Material *material, const BinaryTreeSet<const Light *> &activeLights)
{
	MeshParams mp(material, activeLights);
	auto it = this->meshPrograms.Find(mp);
	if(it != this->meshPrograms.end())
		return (*it).value;

	ShaderProgram *program = this->CompileMeshProgram(mp);
	this->meshPrograms[mp] = program;

	//set shader constants
	program->SetUniformValue(program->GetUniformId("material.tex"), (int32)TEXTURE_UNIT_MATERIAL);
	program->SetUniformValue(program->GetUniformId(u8"environment"), (int32)TEXTURE_UNIT_ENVIRONMENT);

	return program;
}

//Private methods
ShaderProgram *ShaderCompiler::CompileMeshProgram(const MeshParams &meshParams)
{
	ByteString fs;

	//header
	fs += "#version 330 core\n";

	//constants
	fs += "const uint NUM_LIGHTS = " + ToString_8Bit(meshParams.activeLights.GetNumberOfElements()) + "u;";
	fs += "const float gamma = 2.2;";

	//structs
	fs += R"(
struct Light
{
	uint type; //0 = Directional light, 1 = Point light, 2 = Spotlight
	vec3 diffuseColor;
	float power;
	sampler2D shadowMap;
	mat4 lightVP;

	//For directional and Spotlight
	vec3 direction;
	//For Point and Spotlight
	vec3 pos;
	//For Spotlight
	float innerConeAngle;
	float openingAngle;
};
struct Material
{
	vec3 diffuseColor;
	sampler2D tex;
};
)";

	//uniforms
	fs += R"(
uniform vec3 ambientLight;
uniform vec3 cameraPos;
uniform samplerCube environment;
uniform Material material;
)";
	if(!meshParams.activeLights.IsEmpty())
		fs += "uniform Light lights[NUM_LIGHTS];";

	//input/output
	fs += R"(
in vec3 fragPos; //in Worldspace
in vec3 fragNormal;
in vec2 fragTexCoords;

out vec4 outColor;
)";

	//light/shadow computation
	if(!meshParams.activeLights.IsEmpty())
	{
		fs += shader_mesh_fs_shadow;
		fs += shader_mesh_fs_light;
	}

	//lights function
	fs += R"(
vec4 ComputeLights()
{
	vec4 combinedLight;

	combinedLight = vec4(0, 0, 0, 1);
)";

	if(!meshParams.activeLights.IsEmpty())
	{
		fs += R"(
for(uint i = 0u; i < NUM_LIGHTS; i++)
{
combinedLight += ComputeLight(i, lights[i]);
}
)";
	}

	fs += "return vec4(ambientLight, 1) + combinedLight;";
	fs += "}";

	//material function
	if(meshParams.material->GetTexture())
	{
		fs += shader_mesh_material_texture;
	}
	else
	{
		fs += shader_mesh_material_color;
	}

	//main function
	fs += R"(
void main()
	{
		outColor = ComputeMaterialColor() * ComputeLights();
		outColor.rgb = pow(outColor.rgb, vec3(1.0 / gamma)); //apply gamma correction
	}
)";

	return this->CompileProgram(shader_mesh_vs, sizeof(shader_mesh_vs), fs.GetC_Str(), fs.GetSize(), nullptr, 0);
}

ShaderProgram *ShaderCompiler::CompileProgram(const char *vsShaderCode, uint32 vsShaderSize, const char *fsShaderCode, uint32 fsShaderSize, const char *gsShaderCode, uint32 gsShaderSize)
{
	Shader *pVertexShader = this->deviceContext.CreateShader(Shader::ShaderType::VertexShader);
	Shader *pFragmentShader = this->deviceContext.CreateShader(Shader::ShaderType::FragmentShader);
	Shader *pGeometryShader = nullptr;
	if(gsShaderCode)
	{
		pGeometryShader = this->deviceContext.CreateShader(Shader::ShaderType::GeometryShader);
	}

	BufferInputStream vsInput(reinterpret_cast<const byte *>(vsShaderCode), vsShaderSize);
	if(!pVertexShader->Compile(vsInput))
	{
		ASSERT(false, pVertexShader->GetCompilationLog());
	}
	BufferInputStream fsInput(reinterpret_cast<const byte *>(fsShaderCode), fsShaderSize);
	if(!pFragmentShader->Compile(fsInput))
	{
		ASSERT(false, pFragmentShader->GetCompilationLog());
	}
	if(pGeometryShader)
	{
		BufferInputStream gsInput(reinterpret_cast<const byte *>(gsShaderCode), gsShaderSize);
		if(!pGeometryShader->Compile(gsInput))
		{
			ASSERT(false, pGeometryShader->GetCompilationLog());
		}
	}

	ShaderProgram *program = this->deviceContext.CreateShaderProgram();

	program->AttachShader(pVertexShader);
	program->AttachShader(pFragmentShader);
	if(pGeometryShader)
		program->AttachShader(pGeometryShader);
	program->Link();

	//shaders are not needed anymore
	program->DetachShader(pVertexShader);
	delete pVertexShader;

	program->DetachShader(pFragmentShader);
	delete pFragmentShader;

	if(pGeometryShader)
	{
		program->DetachShader(pGeometryShader);
		delete pGeometryShader;
	}

	return program;
}