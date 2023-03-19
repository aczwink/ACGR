#pragma once
//Local
#include "Definitions.h"

namespace CurseGine
{
	//Forward Declarations
	class CMatrix4x4;

	class CURSEGINE_API CVector3
	{
	public:
		//Variables
		float x;
		float y;
		float z;
		//Constructors
		CVector3();
		CVector3(float x, float y, float z);
		//Operators
		void operator+=(const CVector3 &right);
		void operator*=(float scalar);
		//Functions
		void Set(float x, float y, float z);
		void TransformCoord(const CMatrix4x4 &matrix);
	};
}