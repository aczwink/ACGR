#pragma once
#include <ACStdLib.h>
//Local
#include "../Definitions.h"

namespace ACGE
{
	enum class ELightType
	{
		Directional,
		Point,
		Spot
	};
	
	class ACGE_API CLight
	{
	public:
		//Members
		ELightType type;
		ACStdLib::Math::CVector3 color;
		float32 power;
		ACStdLib::Math::CVector3 direction; //For directional and Spotlight
		ACStdLib::Math::CVector3 position; //For Point and Spotlight
		ACStdLib::CDegree openingAngle; //For Spotlight
		ACStdLib::CDegree innerConeAngle; //For Spotlight
		
		//Constructor
		inline CLight()
		{
			this->type = ELightType::Point;
			this->power = 1;
		}

		//Inline
		inline void LookAt(const ACStdLib::Math::CVector3 &refTarget)
		{
			this->direction = refTarget - this->position;
		}
	};
}