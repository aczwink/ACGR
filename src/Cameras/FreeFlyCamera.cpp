/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACGR.
 *
 * ACGR is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACGR is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACGR.  If not, see <http://www.gnu.org/licenses/>.
 */
//Class header
#include <ACGR/Cameras/FreeFlyCamera.hpp>
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Math;
using namespace ACStdLib::UI;
using namespace ACGR;

//Public methods
void FreeFlyCamera::UpdateByMouse(float32 dt)
{
	float32 dx, dy;
	uint16 mouseX, mouseY;
	Size size;

	if(!GetMouseButtonState(MouseButton::Left) && !this->freeLook)
		return;

	const float32 mouseSensitivity = 0.3f * dt;

	//get distance to center of screen
	size = Size::GetScreenSize();
	GetMousePointerPos(mouseX, mouseY);

	dx = (float32)mouseX - (size.width / 2);
	dy = -(float32)mouseY + (size.height / 2);

	dx *= mouseSensitivity;
	dy *= mouseSensitivity;

	this->Rotate(dx, dy);
}

void FreeFlyCamera::UpdateByKeyboard(float32 dt)
{
	float32 camSpeed;
	Vector3 viewDirection, rightDirection;

	//speed
	camSpeed = 4 * dt;
	if(GetKeyState(KeyCode::Shift_Left) || GetKeyState(KeyCode::Shift_Right))
		camSpeed *= 2;

	viewDirection = this->GetForwardDirection();
	rightDirection = this->GetRightDirection();

	//update pos by keyboard
	if(GetKeyState(KeyCode::W))
		this->Translate(-camSpeed * viewDirection);
	if(GetKeyState(KeyCode::S))
		this->Translate(camSpeed * viewDirection);
	if(GetKeyState(KeyCode::A))
		this->Translate(-camSpeed * rightDirection);
	if(GetKeyState(KeyCode::D))
		this->Translate(camSpeed * rightDirection);
}