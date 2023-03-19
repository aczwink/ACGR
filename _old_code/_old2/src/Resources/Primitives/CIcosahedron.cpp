//Class header
#include "CIcosahedron.h"
//Local
#include "__Shared.h"
//Namespaces
using namespace ACGE;

const float32 phi = (1 + sqrtf(5)) / 2;

static const CMesh::SVertex g_icosahedronVertices[] =
{
	{{-1, phi, 0}},
	{{1, phi, 0}},
	{{-1, -phi, 0}},
	{{1, -phi, 0}},

	{{0, -1, phi}},
	{{0, 1, phi}},
	{{0, -1, -phi}},
	{{0, 1, -phi}},

	{{phi, 0, -1}},
	{{phi, 0, 1}},
	{{-phi, 0, -1}},
	{{-phi, 0, 1}},
};

static const uint16 g_icosahedronIndices[] =
{
	0, 11, 5,
	0, 5, 1,
	0, 1, 7,
	0, 7, 10,
	0, 10, 11,

	1, 5, 9,
	5, 11, 4,
	11, 10, 2,
	10, 7, 6,
	7, 1, 8,

	3, 9, 4,
	3, 4, 2,
	3, 2, 6,
	3, 6, 8,
	3, 8, 9,

	4, 9, 5,
	2, 4, 11,
	6, 2, 10,
	8, 6, 7,
	9, 8, 1
};

//Constructor
CIcosahedron::CIcosahedron()
{
	this->SetVertices(12, g_icosahedronVertices);
	this->SetIndices(60, g_icosahedronIndices);
	
	ComputeNormals(this->GetNumberOfVertices(), this->GetVertices(), this->GetNumberOfIndices(), this->GetIndices16());
	this->bbox = ComputeBBox(this->GetNumberOfVertices(), this->GetVertices());
}