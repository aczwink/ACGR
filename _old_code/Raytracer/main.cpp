#include <ACStdLib.h>
using namespace ACStdLib;
//Local
#include "CSphere.h"

//Global variables
CArray<CSceneObject *> g_objects;
CVector3 g_camPos;
CVector3 g_lightPos;
float32 g_lightPower;

CVector3 ComputeShading(const CVector3 &refPos, const CVector3 &refNormal, const CVector3 &refSurfaceColor)
{
	float32 angle, distance;
	CVector3 result, directionToLight;
	
	directionToLight = (refPos - g_lightPos).Normalize();
	angle = refNormal * directionToLight;
	angle = CLAMP(angle, 0, 1);

	distance = (g_lightPos - refPos).Length();

	result = refSurfaceColor * angle * (g_lightPower / (distance * distance));
	//result = refSurfaceColor;

	result.x = CLAMP(result.x, 0, 1);
	result.y = CLAMP(result.y, 0, 1);
	result.z = CLAMP(result.z, 0, 1);

	return result;
}

void ShootRay(const CVector3 &refOrigin, const CVector3 &refDirection, CVector3 &refColor)
{
	CVector3 hitPoint;
	
	for(const CSceneObject *const& refpObject : g_objects)
	{
		if(refpObject->IntersectsLine(refOrigin, refDirection, hitPoint))
		{
			refColor = ComputeShading(hitPoint, refpObject->GetNormal(hitPoint), refpObject->surfaceColor);
			//refColor = ComputeShading(hitPoint, refpObject->GetNormal(hitPoint), refpObject->GetNormal(hitPoint));
		}
		else
		{
			refColor = CVector3(0, 0, 0);
		}
	}
}

void RayTrace()
{
	uint16 width, height, x, y;
	float32 left, right, top, bottom;
	CVector3 direction, currentColor;
	
	CFileOutputStream file(CPath("raytrace.ppm"));
	
	width = 400;
	height = 400;

	left = -0.5f;
	right = 0.5f;
	top = -0.5f;
	bottom = 0.5f;
	
	//write the header
	file << "P6\n" << width << " " << height << "\n255\n";
	
	for(y = 0; y < height; y++)
	{
		for(x = 0; x < width; x++)
		{
			direction.x = left + (x / (float32)width) * (right - left);
			direction.y = top + ((height - y) / (float32)height) * (bottom - top);
			direction.z = -1;
			
			ShootRay(g_camPos, direction.Normalize(), currentColor);
		}
	}
}

void SetupScene()
{
	CSphere *pShere;

	g_camPos = CVector3(0, 0, 1);

	g_lightPos = CVector3(2, 1, 2);
	g_lightPower = 10;
	
	//sphere at center
	pShere = new CSphere(CVector3(0, 0, -0.5f), 0.5f);
	pShere->surfaceColor = CVector3(1.0f, 0, 0);
	g_objects.Push(pShere);
}

int32 Main(const CUTF32String &refProgramName, const CLinkedList<CUTF32String> &refArgs)
{
	SetupScene();
	RayTrace();

	//clean up
	for(const CSceneObject *const& refpObject : g_objects)
	{
		delete refpObject;
	}
	
	return EXIT_SUCCESS;
}