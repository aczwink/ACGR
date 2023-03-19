//Class header
#include "../../headers/Scene/CFreeFlyCamera.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::UI;
using namespace ACGE;

//Constructor
CFreeFlyCamera::CFreeFlyCamera()
{
	this->freeLook = false;
}

//Public methods
void CFreeFlyCamera::UpdateCursorPos(float32 dt)
{
	float32 dx, dy;
	uint16 mouseX, mouseY;
	CSize size;

	if(!GetMouseButtonState(EMouseButton::LEFT) && !this->freeLook)
		return;

	const float32 mouseSensitivity = 0.3f * dt;

	//get distance to center of screen
	size = CSize::GetScreenSize();
	GetCursorPos(mouseX, mouseY);

	dx = -(float32)mouseX + size.width / 2;
	dy = (float32)mouseY - size.height / 2; //screen coordinates are inverted from open gl

	dx *= mouseSensitivity;
	dy *= mouseSensitivity;

	this->Rotate(dx, dy);
}