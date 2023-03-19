//Class Header
#include "CMatrix4x4.h"
//Global
#include <cmath>
//SJCLib
#include <SJCLib.h>
//Namespaces
using namespace SJCLib;
using namespace CurseGine;

//Operators
void CMatrix4x4::operator*=(const CMatrix4x4 &right)
{
	this->Multiply(right);
}

//Public Functions
void CMatrix4x4::Multiply(const CMatrix4x4 &matrix)
{
	CMatrix4x4 tmp;

	//Row 1
	tmp.m11 = this->m11 * matrix.m11 + this->m12 * matrix.m21 + this->m13 * matrix.m31 + this->m14 * matrix.m41;
	tmp.m12 = this->m11 * matrix.m12 + this->m12 * matrix.m22 + this->m13 * matrix.m32 + this->m14 * matrix.m42;
	tmp.m13 = this->m11 * matrix.m13 + this->m12 * matrix.m23 + this->m13 * matrix.m33 + this->m14 * matrix.m43;
	tmp.m14 = this->m11 * matrix.m14 + this->m12 * matrix.m24 + this->m13 * matrix.m34 + this->m14 * matrix.m44;
	//Row 2
	tmp.m21 = this->m21 * matrix.m11 + this->m22 * matrix.m21 + this->m23 * matrix.m31 + this->m24 * matrix.m41;
	tmp.m22 = this->m21 * matrix.m12 + this->m22 * matrix.m22 + this->m23 * matrix.m32 + this->m24 * matrix.m42;
	tmp.m23 = this->m21 * matrix.m13 + this->m22 * matrix.m23 + this->m23 * matrix.m33 + this->m24 * matrix.m43;
	tmp.m24 = this->m21 * matrix.m14 + this->m22 * matrix.m24 + this->m23 * matrix.m34 + this->m24 * matrix.m44;
	//Row 3
	tmp.m31 = this->m31 * matrix.m11 + this->m32 * matrix.m21 + this->m33 * matrix.m31 + this->m34 * matrix.m41;
	tmp.m32 = this->m31 * matrix.m12 + this->m32 * matrix.m22 + this->m33 * matrix.m32 + this->m34 * matrix.m42;
	tmp.m33 = this->m31 * matrix.m13 + this->m32 * matrix.m23 + this->m33 * matrix.m33 + this->m34 * matrix.m43;
	tmp.m34 = this->m31 * matrix.m14 + this->m32 * matrix.m24 + this->m33 * matrix.m34 + this->m34 * matrix.m44;
	//Row 4
	tmp.m41 = this->m41 * matrix.m11 + this->m42 * matrix.m21 + this->m43 * matrix.m31 + this->m44 * matrix.m41;
	tmp.m42 = this->m41 * matrix.m12 + this->m42 * matrix.m22 + this->m43 * matrix.m32 + this->m44 * matrix.m42;
	tmp.m43 = this->m41 * matrix.m13 + this->m42 * matrix.m23 + this->m43 * matrix.m33 + this->m44 * matrix.m43;
	tmp.m44 = this->m41 * matrix.m14 + this->m42 * matrix.m24 + this->m43 * matrix.m34 + this->m44 * matrix.m44;

	*this = tmp;
}

//This algorithm is equal to RotationZ(roll) * RotationX(pitch) * RotationY(yaw)
void CMatrix4x4::RotationYawPitchRoll(float yaw, float pitch, float roll)
{
	this->m11 = cos(roll)*cos(yaw) + sin(roll)*sin(pitch)*sin(yaw);
	this->m12 = sin(roll)*cos(pitch);
	this->m13 = -cos(roll)*sin(yaw) + sin(roll)*sin(pitch)*cos(yaw);
	this->m14 = 0;
	this->m21 = -sin(roll)*cos(yaw) + cos(roll)*sin(pitch)*sin(yaw);
	this->m22 = cos(roll)*cos(pitch);
	this->m23 = sin(roll)*sin(yaw) + cos(roll)*sin(pitch)*cos(yaw);
	this->m24 = 0;
	this->m31 = sin(yaw)*cos(pitch);
	this->m32 = -sin(pitch);
	this->m33 = cos(pitch)*cos(yaw);
	this->m34 = 0;
	this->m41 = 0;
	this->m42 = 0;
	this->m43 = 0;
	this->m44 = 1;
}

//Non-Class Functions
CMatrix4x4 CurseGine::operator*(const CMatrix4x4 &left, const CMatrix4x4 &right)
{
	CMatrix4x4 matrix;

	matrix = left;
	matrix.Multiply(right);

	return matrix;
}