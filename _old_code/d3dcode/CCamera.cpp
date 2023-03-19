//Class Header
#include "CCamera.h"
//Global
#include <d3dx9.h>
//Namespaces
using namespace CurseGine;

//Public Functions
const CMatrix4x4 &CCamera::GetViewMatrix() const
{
	return this->viewMatrix;
}

void CCamera::SetPosition(const CVector3 &position)
{
	this->position = position;
}

void CCamera::Update()
{
	CVector3 up, lookAt;
	float pitch, yaw, roll;
	CMatrix4x4 rotationMatrix;

	up.Set(0, 1, 0);
	lookAt.Set(0, 0, 1);

	pitch = DEG2RAD(this->rotation.x);
	yaw = DEG2RAD(this->rotation.y);
	roll = DEG2RAD(this->rotation.z);

	rotationMatrix.RotationYawPitchRoll(yaw, pitch, roll);
	lookAt.TransformCoord(rotationMatrix);
	up.TransformCoord(rotationMatrix);
	
	lookAt += this->position;

	this->viewMatrix.LookAtLH(this->position, lookAt, up);
}