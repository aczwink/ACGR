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

	class ACGE_API CIndexBuffer
	{
	private:
		//Members
		ID3D11Buffer *pBuffer;
	public:
		//Constructor
		CIndexBuffer(uint32 nIndices, void *pIndices, CD3D &refD3D);
		//Destructor
		~CIndexBuffer();
	};
}