/*
 * Copyright (c) 2017-2019 Amir Czwink (amir130@hotmail.de)
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
using namespace StdXX;
using namespace StdXX::Math;
using namespace StdXX::UI;
using namespace ACGR;

//Public methods
void FreeFlyCamera::UpdateByMouse(float32 dt, const Point<uint16>& mousePos, bool isLeftMouseButtonDown)
{
	float32 dx, dy;
	SizeS size;

	if(!isLeftMouseButtonDown && !this->freeLook)
		return;

	const float32 mouseSensitivity = 0.3f * dt;

	//get distance to center of screen
	NOT_IMPLEMENTED_ERROR; //TODO: line below
	//size = SizeF::GetScreenSize();

	dx = (float32)mousePos.x - (size.width / 2);
	dy = -(float32)mousePos.y + (size.height / 2);

	dx *= mouseSensitivity;
	dy *= mouseSensitivity;

	this->Rotate(dx, dy);
}

void FreeFlyCamera::UpdateByKeyboard(float32 dt, bool isShiftDown, bool w, bool s, bool a, bool d)
{
	float32 camSpeed;
	Vector3S viewDirection, rightDirection;

	//speed
	camSpeed = 4 * dt;
	if(isShiftDown)
		camSpeed *= 2;

	viewDirection = this->GetForwardDirection();
	rightDirection = this->GetRightDirection();

	//update pos by keyboard
	if(w)
		this->Translate(-camSpeed * viewDirection);
	if(s)
		this->Translate(camSpeed * viewDirection);
	if(a)
		this->Translate(-camSpeed * rightDirection);
	if(d)
		this->Translate(camSpeed * rightDirection);
}