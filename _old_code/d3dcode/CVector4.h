#pragma once
//Local
#include "Definitions.h"

namespace CurseGine
{
	class CURSEGINE_API CVector4
	{
	public:
		//Variables
		float x;
		float y;
		float z;
		float w;
		//Constructors
		CVector4();
		CVector4(float x, float y, float z, float w);
	};
}