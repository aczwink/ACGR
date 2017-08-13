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
//Class header
#include "ShaderCompiler.hpp"

//Destructor
ShaderCompiler::~ShaderCompiler()
{
	for(const auto &refKV : this->staticPrograms)
		delete refKV.value;
}

//Public methods
ShaderProgram *ShaderCompiler::FetchStaticProgram(const String &refName)
{
	if(!this->staticPrograms.Contains(refName))
	{
		ShaderProgram *pProgram;
		Shader *pVertexShader, *pFragmentShader, *pGeometryShader;
		Path path;

		path = Path("shaders") / Path(refName);

		/*
		at least a vertex and a pixel shader MUST exist
		they are not needed anymore after program is linked
		*/

		pVertexShader = this->deviceContext.CreateShader(Shader::ShaderType::VertexShader);
		pFragmentShader = this->deviceContext.CreateShader(Shader::ShaderType::FragmentShader);
		pGeometryShader = nullptr;
		if((path / Path("GS.glsl")).Exists())
		{
			pGeometryShader = this->deviceContext.CreateShader(Shader::ShaderType::GeometryShader);
		}

		FileInputStream vsFile(path / Path("VS.glsl"));
		if(!pVertexShader->Compile(vsFile))
		{
			ASSERT_MSG(false, pVertexShader->GetCompilationLog().GetC_Str());
		}
		FileInputStream fsFile(path / Path("FS.glsl"));
		if(!pFragmentShader->Compile(fsFile))
		{
			ASSERT_MSG(false, pFragmentShader->GetCompilationLog().GetC_Str());
		}
		if(pGeometryShader)
		{
			FileInputStream gsFile(path / Path("GS.glsl"));
			if(!pGeometryShader->Compile(gsFile))
			{
				ASSERT_MSG(false, pGeometryShader->GetCompilationLog().GetC_Str());
			}
		}

		pProgram = this->deviceContext.CreateShaderProgram();

		pProgram->AttachShader(pVertexShader);
		pProgram->AttachShader(pFragmentShader);
		if(pGeometryShader)
			pProgram->AttachShader(pGeometryShader);
		pProgram->Link();

		//shaders are not needed anymore
		pProgram->DetachShader(pVertexShader);
		delete pVertexShader;

		pProgram->DetachShader(pFragmentShader);
		delete pFragmentShader;

		if(pGeometryShader)
		{
			pProgram->DetachShader(pGeometryShader);
			delete pGeometryShader;
		}

		this->staticPrograms[refName] = pProgram;
		return pProgram;
	}

	return this->staticPrograms[refName];
}