//Main header
#include "Parse.h"
//Namespaces
using namespace ACStdLib;
using namespace ACStdLib::Math;

//Global functions
CVector3 ParseVec3(const CString &refString)
{
	uint8 i;
	uint16 j;
	CString tmp;
	CVector3 result;

	j = 0;
	for(i = 0; i < 3; i++)
	{
		tmp = "";
		while(j < refString.GetLength())
		{
			if(refString[j] == ',')
				break;
			tmp += (uint32)refString[j];
			j++;
		}
		j++;

		result.e[i] = (float32)StringToFloat64(tmp);
	}

	return result;
}