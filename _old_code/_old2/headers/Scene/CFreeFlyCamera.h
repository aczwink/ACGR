#pragma once
//Local
#include "CCamera.h"

namespace ACGE
{
	class ACGE_API CFreeFlyCamera : public CCamera
	{
	private:
		//Members
		bool freeLook;

	public:
		//Constructor
		CFreeFlyCamera();

		//Methods
		void UpdateCursorPos(float32 dt);
		
		//Inline
		inline void EnableFreeLook(bool enable)
		{
			this->freeLook = enable;
		}

		inline bool IsFreeLookEnabled() const
		{
			return this->freeLook;
		}
	};
}