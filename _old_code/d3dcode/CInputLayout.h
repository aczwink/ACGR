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

	class ACGE_API CInputLayout
	{
	private:
		//Members
		ID3D11InputLayout *pInputLayout;
	public:
		//Constructor
		CInputLayout(uint32 nLayoutEntries, D3D11_INPUT_ELEMENT_DESC *pLayoutDesc, ID3DBlob *pVertexShaderBuffer, CD3D &refD3D);
		//Destructor
		~CInputLayout();
	};
}