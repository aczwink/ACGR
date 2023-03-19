//Class Header
#include "CImage.h"

//Constructor
CImage::CImage(uint16 width, uint16 height)
{
	this->width = width;
	this->height = height;
	
	this->pRed = (byte *)MemAlloc(this->GetNumberOfPixels());
	this->pGreen = (byte *)MemAlloc(this->GetNumberOfPixels());
	this->pBlue = (byte *)MemAlloc(this->GetNumberOfPixels());
}

CImage::CImage(CImage &&refImage)
{
	this->pBlue = nullptr;
	this->pGreen = nullptr;
	this->pRed = nullptr;

	*this = (CImage &&)refImage;
}

//Destructor
CImage::~CImage()
{
	this->Release();
}

//Operators
CImage &CImage::operator = (const CImage &refImage)
{
	this->Release();
	
	this->width = refImage.width;
	this->height = refImage.height;
	
	this->pRed = (byte *)MemAlloc(this->GetNumberOfPixels());
	this->pGreen = (byte *)MemAlloc(this->GetNumberOfPixels());
	this->pBlue = (byte *)MemAlloc(this->GetNumberOfPixels());

	MemCopy(this->pRed, refImage.pRed, this->GetNumberOfPixels());
	MemCopy(this->pGreen, refImage.pGreen, this->GetNumberOfPixels());
	MemCopy(this->pBlue, refImage.pBlue, this->GetNumberOfPixels());
	
	return *this;
}

CImage &CImage::operator=(CImage &&refImage)
{
	this->Release();

	this->width = refImage.width;
	this->height = refImage.height;

	this->pBlue = refImage.pBlue;
	this->pGreen = refImage.pGreen;
	this->pRed = refImage.pRed;

	refImage.pBlue = nullptr;
	refImage.pGreen = nullptr;
	refImage.pRed = nullptr;

	return *this;
}

void CImage::Release()
{
	if(this->pRed)
		MemFree(this->pRed);
	if(this->pGreen)
		MemFree(this->pGreen);
	if(this->pBlue)
		MemFree(this->pBlue);
}

//Public methods
void CImage::BitBlt(uint16 atX, uint16 atY, const CImage &refSource)
{
	byte r, g, b;
	uint16 x, y;

	for(y = 0; y < refSource.GetHeight(); y++)
	{
		for(x = 0; x < refSource.GetWidth(); x++)
		{
			refSource.GetPixel(x, y, r, g, b);
			this->SetPixel(atX + x, atY + y, r, g, b);
		}
	}
}