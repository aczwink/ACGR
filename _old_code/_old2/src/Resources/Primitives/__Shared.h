#pragma once
#include <ACStdLib.h>
//Local
#include "../../../headers/Resources/CMesh.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Math;
using namespace ACGE;

//Global functions
AxisAlignedBox ComputeBBox(uint32 nVertices, const CMesh::SVertex *pVertices);
void ComputeNormals(uint32 nVertices, CMesh::SVertex *pVertices, uint32 nIndices, uint16 *pIndices);