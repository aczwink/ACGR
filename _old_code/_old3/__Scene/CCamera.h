#pragma once

namespace ACGE
{
	class ACGE_API CCamera
	{
		//Methods
		ACStdLib::Math::CMatrix4x4 GetFrustum() const;
	
	public:		
		//Methods
		ACStdLib::Math::CVector3 GetForwardDirection() const;
		ACStdLib::Math::CVector3 GetRightDirection() const;

		//Inline
		inline void Translate(const ACStdLib::Math::CVector3 &refVector)
		{
			this->position += refVector;
		}
	};
}