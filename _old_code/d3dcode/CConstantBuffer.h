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

	class ACGE_API CConstantBuffer
	{
	private:
		//Members
		ID3D11Buffer *pBuffer;
		CD3D &refD3D;
	public:
		//Constructor
		CConstantBuffer(uint32 size, CD3D &refD3D);
		//Destructor
		~CConstantBuffer();
		//Methods
		void Update(const void *pData);
	};
}