//Libs
#include <ACStdLib.h>
//Local
#include "CVector3.h"
#include "CVector4.h"
#include "Definitions.h"

namespace ACGE
{
	class ACGE_API CMatrix4x4
	{
	public:
		//Members
		CVector4 rows[4];

		//Inline operators
		inline CVector4 &operator[](uint8 index)
		{
			ASSERT(index < 4);
			
			return this->rows[index];
		}

		inline const CVector4 &operator[](uint8 index) const
		{
			ASSERT(index < 4);

			return this->rows[index];
		}

		//Methods
		CMatrix4x4 Transpose();

		//Functions
		static CMatrix4x4 Identity();
		static CMatrix4x4 LookAtLH(const CVector3 &refEyePosition, const CVector3 &refFocusPosition, const CVector3 &refUpDirection);
		static CMatrix4x4 PerspectiveFovLH(float32 fovAngleY, float32 aspectRatio, float32 nearZ, float32 farZ);
		static CMatrix4x4 RotationPitchYawRoll(float32 angleX, float32 angleY, float32 angleZ);
		static CMatrix4x4 RotationX(float32 angle);
		static CMatrix4x4 RotationY(float32 angle);
		static CMatrix4x4 RotationZ(float32 angle);
		static CMatrix4x4 Scale(float32 scaleX, float32 scaleY, float32 scaleZ);
		static CMatrix4x4 Translation(float32 offsetX, float32 offsetY, float32 offsetZ);
	};

	//Matrix,Matrix operators
	ACGE_API CMatrix4x4 operator*(const CMatrix4x4 &refLeft, const CMatrix4x4 &refRight);
}