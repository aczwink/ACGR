//Class header
#include "CPlane.h"
//Local
#include "__Shared.h"
//Namespaces
using namespace ACGE;

static const CMesh::SVertex g_planePositions[] =
{
	//left
	{{-0.5f, 0.5f, 0}, {0, 0, 1}, {0, 1}}, //top
	{{-0.5f, -0.5f, 0}, {0, 0, 1}, {0, 0}}, //bottom
	
	//right
	{{0.5f, 0.5f, 0}, {0, 0, 1}, {1, 1}}, //top
	{{0.5f, -0.5f, 0}, {0, 0, 1}, {1, 0}} //bottom
};

static const uint16 g_planeIndices[] =
{
	0, 1, 3, //lower
	0, 3, 2, //upper
};

//Constructor
CPlane::CPlane()
{
	this->SetIndices(6, g_planeIndices);
	this->SetVertices(4, g_planePositions);

	this->bbox = ComputeBBox(this->GetNumberOfVertices(), this->GetVertices());
}