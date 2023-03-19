//Class header
#include "CCuboid.h"
//Namespaces
using namespace ACGE;

static const CMesh::SVertex g_boxPositions[] =
{
	//left
	{{-0.5f, 0.5f, -0.5f}, {-1, 0, 0}, {0, 1}},//top back
	{{-0.5f, -0.5f, -0.5f}, {-1, 0, 0}, {0, 0}}, //bottom back
	{{-0.5f, -0.5f, 0.5f}, {-1, 0, 0}, {1, 0}}, //bottom front
	{{-0.5f, 0.5f, 0.5f}, {-1, 0, 0}, {1, 1}},//top front

	//right
	{{0.5f, 0.5f, 0.5f}, {1, 0, 0}, {0, 1}},//top front
	{{0.5f, -0.5f, 0.5f}, {1, 0, 0}, {0, 0}}, //bottom front
	{{0.5f, -0.5f, -0.5f}, {1, 0, 0}, {1, 0}}, //bottom back
	{{0.5f, 0.5f, -0.5f}, {1, 0, 0}, {1, 1}},//top back

	//bottom
	{{0.5f, -0.5f, 0.5f}, {0, -1, 0}, {0, 1}}, //right front
	{{-0.5f, -0.5f, 0.5f}, {0, -1, 0}, {0, 0}}, //left front
	{{-0.5f, -0.5f, -0.5f}, {0, -1, 0}, {1, 0}}, //left back
	{{0.5f, -0.5f, -0.5f}, {0, -1, 0}, {1, 1}}, //right back

	//top
	{{-0.5f, 0.5f, -0.5f}, {0, 1, 0}, {0, 1}}, //left back
	{{-0.5f, 0.5f, 0.5f}, {0, 1, 0}, {0, 0}}, //left front
	{{0.5f, 0.5f, 0.5f}, {0, 1, 0}, {1, 0}}, //right front
	{{0.5f, 0.5f, -0.5f}, {0, 1, 0}, {1, 1}}, //right back

	//back
	{{0.5f, 0.5f, -0.5f}, {0, 0, -1}, {0, 1}}, //top right
	{{0.5f, -0.5f, -0.5f}, {0, 0, -1}, {0, 0}}, //bottom right
	{{-0.5f, -0.5f, -0.5f}, {0, 0, -1}, {1, 0}}, //bottom left
	{{-0.5f, 0.5f, -0.5f}, {0, 0, -1}, {1, 1}},//top left

	//front
	{{-0.5f, 0.5f, 0.5f}, {0, 0, 1}, {0, 1}},//top left
	{{-0.5f, -0.5f, 0.5f}, {0, 0, 1}, {0, 0}}, //bottom left
	{{0.5f, -0.5f, 0.5f}, {0, 0, 1}, {1, 0}}, //bottom right
	{{0.5f, 0.5f, 0.5f},{0, 0, 1}, {1, 1}}, //top right
};

static const uint16 g_boxIndices[] =
{
	//left
	0, 1, 2, //back
	0, 2, 3, //front

	//right
	4, 5, 6, //front
	4, 6, 7, //back

	//bottom
	8, 9, 10, //front
	8, 10, 11, //back
	
	//top
	12, 13, 14, //back
	12, 14, 15, //front

	//back
	16, 17, 18, //left
	16, 18, 19, //right

	//front
	20, 21, 22, //left
	20, 22, 23, //right
};

//Constructor
CCuboid::CCuboid()
{
	this->SetIndices(sizeof(g_boxIndices)/sizeof(g_boxIndices[0]), g_boxIndices);
	this->SetVertices(sizeof(g_boxPositions)/sizeof(g_boxPositions[0]), g_boxPositions);
}