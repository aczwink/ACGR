#pragma once
#include <ACStdLib.h>
//Local
#include "../Definitions.h"

namespace ACGE
{
	class ACGE_API CCamera
	{
	private:
		//Members
		ACStdLib::CDegree horzAngle; //Clockwise (straight = 0), range [0, 360]
		ACStdLib::CDegree vertAngle; //Clockwise (straight = 0, positive values = look down), range [-85, 85]
		ACStdLib::CDegree fieldOfViewY;
		float32 aspectRatio;
		float32 nearPlane;
		float32 farPlane;

		//Methods
		ACStdLib::Math::CMatrix4x4 GetOrientation() const;
		void NormalizeAngles();

	public:
		//Members
		ACStdLib::Math::CVector3 position;

		//Constructor
		CCamera();

		//Methods
		ACStdLib::Math::CMatrix4x4 GetPerspectiveMatrix() const;
		ACStdLib::Math::CMatrix4x4 GetViewMatrix() const;
		void SetViewDirection(const ACStdLib::Math::CVector3 &refDir);

		//Inline
		inline void LookAt(const ACStdLib::Math::CVector3 &refTarget)
		{
			this->SetViewDirection(refTarget - this->position);
		}
		
		inline void Rotate(const ACStdLib::CDegree &refDx, const ACStdLib::CDegree &refDy)
		{
			this->horzAngle += refDx;
			this->vertAngle += refDy;

			this->NormalizeAngles();
		}

		inline void SetAspectRatio(float32 aspectRatio)
		{
			this->aspectRatio = aspectRatio;
		}

		inline void SetFieldOfView(const ACStdLib::CDegree &refFieldOfView)
		{
			this->fieldOfViewY = refFieldOfView;
		}
	};
}