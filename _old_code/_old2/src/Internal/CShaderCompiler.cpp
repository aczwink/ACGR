//Class header
#include "CShaderCompiler.h"

//Destructor
CShaderCompiler::~CShaderCompiler()
{
	for(const auto &refKV : this->staticPrograms)
		delete refKV.value;
}

//Public methods
IShaderProgram *CShaderCompiler::FetchStaticProgram(const CString &refName)
{
	if(!this->staticPrograms.Contains(refName))
	{
		IShaderProgram *pProgram;
		IShader *pVertexShader, *pFragmentShader, *pGeometryShader;
		CPath path;
		
		path = CPath("shaders") / CPath(refName);

		/*
		at least a vertex and a pixel shader MUST exist
		they are not needed anymore after program is linked
		*/

		pVertexShader = this->refDeviceContext.CreateShader(IShader::EShaderType::VertexShader);
		pFragmentShader = this->refDeviceContext.CreateShader(IShader::EShaderType::FragmentShader);
		pGeometryShader = nullptr;
		if((path / CPath("GS.glsl")).Exists())
		{
			pGeometryShader = this->refDeviceContext.CreateShader(IShader::EShaderType::GeometryShader);
		}

		if(!pVertexShader->Compile(CFileInputStream(path / CPath("VS.glsl"))))
		{
			ASSERT_MSG(false, pVertexShader->GetCompilationLog().GetC_Str());
		}
		if(!pFragmentShader->Compile(CFileInputStream(path / CPath("FS.glsl"))))
		{
			ASSERT_MSG(false, pFragmentShader->GetCompilationLog().GetC_Str());
		}
		if(pGeometryShader && !pGeometryShader->Compile(CFileInputStream(path / CPath("GS.glsl"))))
		{
			ASSERT_MSG(false, pGeometryShader->GetCompilationLog().GetC_Str());
		}

		pProgram = this->refDeviceContext.CreateShaderProgram();

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