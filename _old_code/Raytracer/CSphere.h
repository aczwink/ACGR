//Local
#include "CSceneObject.h"

class CSphere : public CSceneObject
{
private:
	//Members
	float32 radius;

public:
	//Constructor
	inline CSphere(const CVector3 &refPosition, float32 radius) : CSceneObject(refPosition)
	{
		this->radius = radius;
	}
	
	//Methods
	bool IntersectsLine(const CVector3 &refLineOrigin, const CVector3 &refLineDirection, CVector3 &refHitPoint) const;
	CVector3 GetNormal(const CVector3 &refPos) const;
};