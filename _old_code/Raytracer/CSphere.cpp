//Class header
#include "CSphere.h"

//Public methods
bool CSphere::IntersectsLine(const CVector3 &refLineOrigin, const CVector3 &refLineDirection, CVector3 &refHitPoint) const
{
	float32 tca, distanceSquared, thcSquared, thc, t1, t2;
	CVector3 distanceToCenter;

	//vector from line origin to center of sphere
	distanceToCenter = this->position - refLineOrigin;
	
	//center of sphere projected onto line
	tca = distanceToCenter * refLineDirection;
	
	//calculate distance squared of line to center of sphere (pythagoras)
	distanceSquared = distanceToCenter * distanceToCenter - tca * tca;

	//distance to hull of sphere
	thcSquared = this->radius * this->radius - distanceSquared;
	
	//check if there is a intersection
	if(thcSquared < 0)
		return false;
	
	thc = sqrtf(thcSquared);
	t1 = tca + thc;
	t2 = tca - thc;

	if(t1 < t2)
	{
		refHitPoint = refLineOrigin + t1 * refLineDirection;
	}
	else
	{
		refHitPoint = refLineOrigin + t2 * refLineDirection;
	}
	
	return true;
}

CVector3 CSphere::GetNormal(const CVector3 &refPos) const
{
	return (this->radius * CVector3(refPos.x, refPos.y, -refPos.z)).Normalize();
}