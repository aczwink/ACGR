//Global
#include <xmmintrin.h>
//Libs
#include <ACStdLib.h>
//Local
#include "Definitions.h"

namespace ACGE
{
	class ACGE_API CColor
	{
	public:
		//Members
		union
		{
			struct
			{
				float32 r;
				float32 g;
				float32 b;
				float32 a;
			};
			float32 c[4];
			__m128 xmm;
		};

		//Constructor
		inline CColor()
		{
			this->xmm = _mm_setzero_ps();
		}

		inline CColor(float32 r, float32 g, float32 b, float32 a)
		{
			this->xmm = _mm_set_ps(a, b, g, r);
		}

		//Casting operators
		inline operator const float32 *() const
		{
			return this->c;
		}
	};
}