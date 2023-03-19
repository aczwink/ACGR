//Class Header
#include "CCamera.h"
/*
//Namespaces

//Public methods
void CCamera::Update(float32 dt)
{
	float32 camSpeed;
	CVector3 viewDirection, rightDirection;

	//speed
	camSpeed = 8 * dt;
	if(GetKeyState(EKeyCode::SHIFT_LEFT) || GetKeyState(EKeyCode::SHIFT_RIGHT))
		camSpeed *= 2;
	
	viewDirection = this->GetForwardDirection();
	rightDirection = this->GetRightDirection();
	
	//update pos by keyboard
	if(GetKeyState(EKeyCode::W))
		this->Translate(camSpeed * viewDirection);
	if(GetKeyState(EKeyCode::S))
		this->Translate(-camSpeed * viewDirection);
	if(GetKeyState(EKeyCode::A))
		this->Translate(-camSpeed * rightDirection);
	if(GetKeyState(EKeyCode::D))
		this->Translate(camSpeed * rightDirection);
}

void CCamera::UpdateFov(int16 delta)
{
	float64 fov;
	
	fov = this->GetFieldOfView().value - delta;
	fov = CLAMP(fov, 1.0f, 45.0f);
	this->SetFieldOfView(fov);
}
*/