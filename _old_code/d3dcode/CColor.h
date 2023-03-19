#pragma once
//Local
#include "Definitions.h"

namespace CurseGine
{
	class CURSEGINE_API CColor
	{
	public:
		//Variables
		float r;
		float g;
		float b;
		float a;
		//Constructor
		CColor(float r, float g, float b, float a);
		//Functions
		const float *GetFloat4() const;
	};
}