#pragma once
//Global
#include <xmmintrin.h>
//Libs
#include <ACStdLib.h>
//Local
#include "Definitions.h"

namespace ACGE
{
	class CVector4
	{
	public:
		//Members
		union
		{
			struct
			{
				float32 x;
				float32 y;
				float32 z;
				float32 w;
			};
			float32 e[4];
			__m128 xmm;
		};

		//Constructors
		inline CVector4()
		{
			this->xmm = _mm_setzero_ps();
		}
		
		inline CVector4(float32 x, float32 y, float32 z, float32 w)
		{
			this->xmm = _mm_set_ps(w, z, y, x);
		}

		//Inline operators
		inline float32 &operator[](uint8 index)
		{
			ASSERT(index < 4);

			return this->e[index];
		}

		inline float32 operator[](uint8 index) const
		{
			ASSERT(index < 4);

			return this->e[index];
		}
	};
}