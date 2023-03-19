//Class header
#include "CTriangle.h"
//Namespaces
using namespace ACGE;

static const CMesh::SVertex g_trianglePositions[] =
{
	{{0, 0.5f, 0}, {0, 0, 1}, {0.5f, 0}}, //top
	{{-0.5f, -0.5f, 0},{0, 0, 1}, {0, 1}}, //left bottom
	{{0.5f, -0.5f, 0},{0, 0, 1}, {1, 1}}, //right bottom
};

static const uint16 g_triangleIndices[] =
{
	0, 1, 2
};

//Constructor
CTriangle::CTriangle()
{
	this->SetIndices(3, g_triangleIndices);
	this->SetVertices(3, g_trianglePositions);
}