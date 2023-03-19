//Class Header
#include "CColor.h"
//Namespaces
using namespace CurseGine;

//Constructor
CColor::CColor(float r, float g, float b, float a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

//Public Functions
const float *CColor::GetFloat4() const
{
	return &this->r;
}