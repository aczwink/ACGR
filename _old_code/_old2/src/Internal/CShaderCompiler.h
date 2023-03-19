#pragma once
#include <ACStdLib.h>
using namespace ACStdLib;
using namespace ACStdLib::Rendering;

class CShaderCompiler
{
private:
	//Members
	CMap<CString, IShaderProgram *> staticPrograms;

	CDeviceContext &refDeviceContext;

public:
	//Constructor
	inline CShaderCompiler(CDeviceContext &refDeviceContext) : refDeviceContext(refDeviceContext)
	{
	}

	//Destructor
	~CShaderCompiler();
	
	//Methods
	IShaderProgram *FetchStaticProgram(const CString &refName);
};