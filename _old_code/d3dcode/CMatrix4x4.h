#pragma once
//Local
#include "CVector3.h"
#include "Definitions.h"

namespace CurseGine
{
	class CURSEGINE_API CMatrix4x4
	{
	public:
		//Variables
		float m11, m12, m13, m14;
		float m21, m22, m23, m24;
		float m31, m32, m33, m34;
		float m41, m42, m43, m44;
		//Operators
		void operator*=(const CMatrix4x4 &right);
		//Functions
		void Multiply(const CMatrix4x4 &matrix);
		void RotationYawPitchRoll(float yaw, float pitch, float roll);
	};

	CMatrix4x4 operator*(const CMatrix4x4 &left, const CMatrix4x4 &right);
}