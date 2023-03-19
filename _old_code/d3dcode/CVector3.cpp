//Class Header
#include "CVector3.h"
//Global
#include <cmath>
//Local
#include "CMatrix4x4.h"
#include "CVector4.h"
//Namespaces
using namespace CurseGine;

//Constructors
CVector3::CVector3()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

CVector3::CVector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

//Operators
void CVector3::operator+=(const CVector3 &right)
{
	this->x += right.x;
	this->y += right.y;
	this->z += right.z;
}

void CVector3::operator*=(float scalar)
{
	this->x *= scalar;
	this->y *= scalar;
	this->z *= scalar;
}

//Public Functions
void CVector3::Set(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void CVector3::TransformCoord(const CMatrix4x4 &matrix)
{
	CVector4 v;

	v.x = this->x * matrix.m11 + this->y * matrix.m21 + this->z * matrix.m31 + matrix.m41;
	v.y = this->x * matrix.m12 + this->y * matrix.m22 + this->z * matrix.m32 + matrix.m42;
	v.z = this->x * matrix.m13 + this->y * matrix.m23 + this->z * matrix.m33 + matrix.m43;
	v.w = this->x * matrix.m14 + this->y * matrix.m24 + this->z * matrix.m34 + matrix.m44;

	this->x = v.x / v.w;
	this->y = v.y / v.w;
	this->z = v.z / v.w;
}