//Class Header
#include "CConfig.h"
//Global
#include <d3d11.h>
//Local
#include "Definitions.h"
//Namespaces
using namespace ACStdLib;
using namespace ACGE;

//Constructor
CConfig::CConfig() : cfgFile(CPath(CString(("ACGE_Config.ini"))))
{
	if(!this->cfgFile.ContainsSection(CONFIG_SECTION_VIDEO)) //empty ini
		this->AutoConfigVideo();
}

//Private methods
void CConfig::AutoConfigVideo()
{
	uint16 screenWidth, screenHeight, bestWidth, bestHeight;
	uint32 nModes;
	IDXGIFactory *pFactory;
	IDXGIAdapter *pAdapter;
	IDXGIOutput *pOutput;
	DXGI_MODE_DESC *pModeDescs;
	
	screenHeight = GetSystemMetrics(SM_CYSCREEN);
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	bestWidth = 640;
	bestHeight = 480;

	this->SetWindowed(true);
	
	//Find adapter first
	if(SUCCEEDED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void **)&pFactory)))
	{
		if(pFactory->EnumAdapters(0, &pAdapter) != DXGI_ERROR_NOT_FOUND) //first adapter is standard hardware adapter
		{
			for(uint32 i = 0; (pAdapter->EnumOutputs(i, &pOutput) != DXGI_ERROR_NOT_FOUND); i++)
			{
				//search for windowed resolutions
				pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &nModes, NULL);
				
				pModeDescs = (DXGI_MODE_DESC *)malloc(nModes * sizeof(*pModeDescs));
				pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &nModes, pModeDescs);
				for(uint32 j = 0; j < nModes; j++)
				{
					if(pModeDescs[j].Width <= (uint16)(screenWidth/2) && pModeDescs[j].Height <= (uint16)(screenHeight/2) && (pModeDescs[j].Height > bestHeight || pModeDescs[j].Width > bestWidth))
					{
						bestWidth = pModeDescs[j].Width;
						bestHeight = pModeDescs[j].Height;
					}
				}
				free(pModeDescs);

				COM_SAFE_RELEASE(pOutput);
			}

			COM_SAFE_RELEASE(pAdapter);
		}
		
		COM_SAFE_RELEASE(pFactory);
	}

	this->SetResolution(bestWidth, bestHeight);
}