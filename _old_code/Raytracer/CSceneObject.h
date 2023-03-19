#include <ACStdLib.h>
using namespace ACStdLib;
using namespace ACStdLib::Math;

class CSceneObject
{
protected:
	//Members
	CVector3 position;

public:
	//Members
	CVector3 surfaceColor;

	//Constructor
	inline CSceneObject(const CVector3 &refPosition) : position(refPosition)
	{
	}

	//Abstract
	virtual bool IntersectsLine(const CVector3 &refLineOrigin, const CVector3 &refRayLineDirection, CVector3 &refHitPoint) const = NULL;
	virtual CVector3 GetNormal(const CVector3 &refPos) const = NULL;
	
	//Destructor
	virtual ~CSceneObject(){}
};