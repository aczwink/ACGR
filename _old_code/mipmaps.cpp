#include <ACStdLib.h>
using namespace ACStdLib;
//Local
#include "CImage.h"

CArray<CImage *> GenerateMipMaps(CImage &refImage)
{
	byte r[4], g[4], b[4];
	uint16 mipMapWidth, mipMapHeight;
	uint32 tmpR, tmpG, tmpB;
	CImage *pSrc, *pDst;
	CArray<CImage *> mipmaps;

	mipmaps.Push(&refImage);

	while(true)
	{
		pSrc = mipmaps[mipmaps.GetNumberOfElements() - 1];
		
		//calculate mip map size
		mipMapWidth = pSrc->GetWidth() / 2;
		mipMapHeight = pSrc->GetHeight() / 2;
		
		if(mipMapWidth == 0 && mipMapHeight == 0)
			break;
		
		if(mipMapWidth < 1)
			mipMapWidth = 1;
		if(mipMapHeight < 1)
			mipMapHeight = 1;
		
		pDst = new CImage(mipMapWidth, mipMapHeight);

		//calculate mip map
		for(uint16 y = 0; y < pDst->GetHeight(); y++)
		{
			for(uint16 x = 0; x < pDst->GetWidth(); x++)
			{
				pSrc->GetPixel(2 * x, 2 * y, r[0], g[0], b[0]);
				pSrc->GetPixel(2 * x + 1, 2 * y, r[1], g[1], b[1]);
				pSrc->GetPixel(2 * x, 2 * (y + 1), r[2], g[2], b[2]);
				pSrc->GetPixel(2 * x + 1, 2 * (y + 1), r[3], g[3], b[3]);

				tmpR = r[0] + r[1] + r[2] + r[3];
				tmpR /= 4;

				tmpG = g[0] + g[1] + g[2] + g[3];
				tmpG /= 4;

				tmpB = b[0] + b[1] + b[2] + b[3];
				tmpB /= 4;

				pDst->SetPixel(x, y, (byte)tmpR, (byte)tmpG, (byte)tmpB);
			}
		}
		
		mipmaps.Push(pDst);
	}

	return mipmaps;
}