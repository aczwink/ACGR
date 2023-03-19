//Class header
#include "../../headers/Scene/CCamera.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Math;
using namespace ACGE;

//Constructor
CCamera::CCamera()
{
	this->horzAngle = 0;
	this->vertAngle = 0;
	this->fieldOfViewY = 0;
	this->aspectRatio = 1;
	this->nearPlane = 0.1f;
	this->farPlane = 1000.0f;
}

//Private methods
CMatrix4x4 CCamera::GetOrientation() const
{
	return CMatrix4x4::RotationYawPitchRoll(this->horzAngle, this->vertAngle, 0);
}

void CCamera::NormalizeAngles()
{
	this->horzAngle.value = fmod(this->horzAngle.value, 360.0f);
	if(this->horzAngle < 0) //fmodf may return less than 0
		this->horzAngle += 360.0f;

	//avoid gimbal lock
	this->vertAngle = CLAMP(this->vertAngle, -85.0f, 85.0f);
}

//Public methods
CMatrix4x4 CCamera::GetPerspectiveMatrix() const
{
	return CMatrix4x4::PerspectiveRH(this->fieldOfViewY, this->aspectRatio, this->nearPlane, this->farPlane);
}

CMatrix4x4 CCamera::GetViewMatrix() const
{
	return this->GetOrientation() * CMatrix4x4::Translation(-this->position);
}

void CCamera::SetViewDirection(const CVector3 &refDir)
{
	CVector3 dirNormalized;
	
	dirNormalized = refDir.Normalize(); //direction to target
	
	this->vertAngle = CRadian(asinf(-dirNormalized.y)); //invert y because positive angles will rotate the camera downwards
													
	/*
	-invert z coordinate, because refTarget and this->position are from an rhs coordinate system
	-atan2 will return a negative value for clockwise angles, which we want. Therefore again negate
	*/
	this->horzAngle = -CRadian(atan2f(-dirNormalized.x, -dirNormalized.z));

	this->NormalizeAngles();
}