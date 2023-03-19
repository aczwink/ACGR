#pragma once
//Global
#include <cmath>
//Libs
#include <ACStdLib.h>
//Local
#include "Definitions.h"

namespace ACGE
{
	class CVector3
	{
	public:
		//Members
		float32 x;
		float32 y;
		float32 z;
		
		//Constructor
		inline CVector3()
		{
			this->x = 0;
			this->y = 0;
			this->z = 0;
		}

		inline CVector3(float32 x, float32 y, float32 z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}

		//Inline operators
		inline CVector3 &operator*(float32 scalar)
		{
			this->x *= scalar;
			this->y *= scalar;
			this->z *= scalar;
			
			return *this;
		}

		//Inline
		inline CVector3 Cross(const CVector3 &refRight) const
		{
			float32 x, y, z;
			
			x = this->y*refRight.z - this->z*refRight.y;
			y = this->z*refRight.x - this->x*refRight.z;
			z = this->x*refRight.y - this->y*refRight.x;
			
			return CVector3(x, y, z);
		}
		
		inline CVector3 GetNorm() const
		{
			float32 length;

			length = 1.0f / this->GetLength();

			return CVector3(*this) * length;
		}

		inline float32 GetLength() const
		{
			return sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
		}
	};

	/*//Scalar, Vector operators
	inline CVector3 operator*(float32 scalar, const CVector3 &refVector)
	{
		return CVector3(scalar * refVector.x, scalar * refVector.y, scalar * refVector.z);
	}

	//Vector, Scalar operators
	inline CVector3 operator*(const CVector3 &refVector, float32 scalar)
	{
		return CVector3(scalar * refVector.x, scalar * refVector.y, scalar * refVector.z);
	}*/

	//Vector, Vector operators
	inline CVector3 operator-(const CVector3 &refLeft, const CVector3 &refRight)
	{
		return CVector3(refLeft.x - refRight.x, refLeft.y - refRight.y, refLeft.z - refRight.z);
	}
	
	inline float32 operator*(const CVector3 &refLeft, const CVector3 &refRight)
	{
		return refLeft.x*refRight.x + refLeft.y*refRight.y + refLeft.z*refRight.z;
	}
}