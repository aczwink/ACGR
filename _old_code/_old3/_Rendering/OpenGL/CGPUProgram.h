#pragma once
#include <ACStdLib.h>
//Local
#include "../../../headers/Definitions.h"

namespace ACGE
{
	namespace OpenGLRenderer
	{
		//Forward declarations
		class AShader;
		
		class CGPUProgram
		{
			friend class CVertexArray;
		private:
			//Members
			uint32 id;
		
		public:				
			//Methods
			void SetUniformValue(const ACStdLib::CString &refVarName, float32 value);

			//Inline
			inline uint32 GetAttributeLocation(const ACStdLib::CString &refVarName) const
			{
				return glGetAttribLocation(this->id, refVarName.GetC_Str());
			}
		};
	}
}