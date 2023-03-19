//Global
#include <d3d11.h>
//Libs
#include <ACStdLib.h>
//Local
#include "Definitions.h"

namespace ACGE
{
	//Forward declarations
	class CD3D;
	
	class ACGE_API CVertexBuffer
	{
	private:
		//Members
		ID3D11Buffer *pBuffer;
	public:
		//Constructor
		CVertexBuffer(uint32 nVertices, uint32 vertexSize, void *pVertices, CD3D &refD3D);
		//Destructor
		~CVertexBuffer();
	};
}