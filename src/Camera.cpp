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
#include <ACGR/Camera.hpp>
//Namespaces
using namespace StdXX;
using namespace StdXX::Math;
using namespace ACGR;

//Constructor
Camera::Camera()
{
	this->horzAngle = 0;
	this->vertAngle = 0;
	this->fieldOfViewY = 0;
	this->aspectRatio = 1;
	this->nearPlane = 0.1f;
	this->farPlane = 1000.0f;
}

//Public methods
Matrix4S Camera::GetPerspectiveMatrix() const
{
	return Matrix4S::PerspectiveRH(this->fieldOfViewY, this->aspectRatio, this->nearPlane, this->farPlane);
}

void Camera::SetViewDirection(const Vector3S &refDir)
{
	Vector3S dirNormalized;

	dirNormalized = refDir.Normalized(); //direction to target

	this->vertAngle = Radian(asinf(-dirNormalized.y)); //invert y because positive angles will rotate the camera downwards

	/*
	-invert z coordinate, because refTarget and this->position are from an rhs coordinate system
	-atan2 will return a negative value for clockwise angles, which we want. Therefore again negate
	*/
	this->horzAngle = -Radian(atan2f(-dirNormalized.x, -dirNormalized.z));

	this->NormalizeAngles();
}

//Private methods
void Camera::NormalizeAngles()
{
	this->horzAngle.value = fmod(this->horzAngle.value, 360.0f);
	if(this->horzAngle < 0) //fmodf may return less than 0
		this->horzAngle += 360.0f;

	//avoid gimbal lock
	this->vertAngle = Clamp(this->vertAngle.value, -85.0f, 85.0f);
}