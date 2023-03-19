//Class Header
#include "CVector4.h"
//Namespaces
using namespace CurseGine;

//Constructors
CVector4::CVector4()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
	this->w = 0;
}

CVector4::CVector4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}